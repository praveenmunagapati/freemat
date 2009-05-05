/*
 * DO NOT EDIT THIS FILE! It was created automatically by m4.
 */

/*
 * Copyright (c) 1995-2008, Stanford University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Stanford University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Author:
 *    Phil Lacroute
 *    Computer Systems Laboratory
 *    Electrical Engineering Dept.
 *    Stanford University
 */
/*
 * vp_warpA.m4
 *
 * One-pass image warping routine for affine transformations.
 *
 * Copyright (c) 1995-2008, Stanford University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Stanford University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY STANFORD UNIVERSITY ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL STANFORD UNIVERSITY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:
 *    Phil Lacroute
 *    Computer Systems Laboratory
 *    Electrical Engineering Dept.
 *    Stanford University
 */

#include "vp_global.h"



    
    
    
    
    

    
    
    









    
    
    
    
    





    
    
    
    
    








	

/* convert a float in the interval [0-1) to a 31-bit fixed point */
#define FLTFRAC_TO_FIX31(f)	((int)((f) * 2147483648.))

/* convert a 31-bit fixed point to a weight table index */
#define FIX31_TO_WGTIND(f)	((f) >> (31 - WARP_WEIGHT_INDEX_BITS))

extern float VPBilirpWeight[WARP_WEIGHT_ENTRIES][WARP_WEIGHT_ENTRIES][4];

/*
 * VPWarpA331R
 *
 * One-pass warper.  Transforms in_image to out_image according to
 * the affine warp specified by warp_matrix.  The resampling filter
 * is a bilirp (suitable for upsampling only).
 */

void
VPWarpA331R (in_image, in_width, in_height, in_bytes_per_scan,
	  out_image, out_width, out_height, out_bytes_per_scan,
	  warp_matrix)
RGBIntPixel *in_image;		/* input image data */
int in_width;			/* size of input image */
int in_height;
int in_bytes_per_scan;		/* bytes per scanline in input image */
char *out_image;		/* output image data */
int out_width;			/* size of output image */
int out_height;
int out_bytes_per_scan;		/* bytes per scanline in output image */
vpMatrix3 warp_matrix;		/* [ outx ]                 [ inx ] */
				/* [ outy ] = warp_matrix * [ iny ] */
				/* [   1  ]                 [  1  ] */
{
    Trapezoid full_overlap[9];	/* description of the area of overlap
				   of output image (shrunk by the size
				   of the filter kernel) with input image */
    Trapezoid part_overlap[9];	/* description of the area of overlap
				   of output image (unlarged by the size
				   of the filter kernel) with input image */
    int region;			/* index into full/part_overlap */
    char *out_ptr;		/* pointer to current pixel of output image */
    int out_scan_y;		/* coordinate of current output scanline */
    int scans_to_next_vertex;	/* number of scans left to process before
				   the next vertex is reached */
    RGBIntPixel *in_ptr;	/* pointer to current pixel of input image */
    double x_lft_full, x_rgt_full; /* intersection of scan with full_overlap */
    double x_lft_part, x_rgt_part; /* intersection of scan with part_overlap */
    int no_full_pixels;		/* true if full_overlap is empty for scan */
    double in_x, in_y;		/* exact coordinates in the input image of
				   the current output image pixel */
    int in_x_int, in_y_int;	/* coordinates of the nearest input image
				   pixel to the upper-left of the current
				   output image pixel */
    int xfrac, yfrac;		/* in_x - in_x_int and in_y - in_y_int,
				   stored as a fixed-point number with 31 bits
				   of fraction */
    int xfrac_incr, yfrac_incr;	/* increments to xfrac and yfrac to give
				   the fractions for the next output image
				   pixel in the current scan */
    double in_x_incr, in_y_incr;/* increments to in_x and in_y to give the
				   input image coordinates of the next
				   output image pixel in the current scan 
				   (equal to dx_in/dx_out and dy_in/dx_out) */
    int in_x_incr_int, in_y_incr_int; /* integer part of in_x/y_incr */
    int in_x_incr_dlt, in_y_incr_dlt; /* sign of in_x/y_incr */
    float *wptr;		/* pointer into weight table */
    int lft_zero_cnt;		/* # zero pixels on left edge of scan */
    int lft_edge_cnt;		/* # pixels on left w/ part filter overlap */
    int full_cnt;		/* # pixels w/ full filter overlap */
    int rgt_edge_cnt;		/* # pixels on rgt w/ part filter overlap */
    int rgt_zero_cnt;		/* # zero pixels on right edge of scan */
    int x;			/* pixel index */
    
	float r_acc, g_acc, b_acc;
    int r_acc_int, g_acc_int, b_acc_int;
	float opc_acc; int opc_acc_int;		/* pixel accumulator */
    double denom;
    int c;

#ifdef DEBUG
    {
	int y;

	for (y = 0; y < out_height; y++) {
	    out_ptr = out_image + y*out_bytes_per_scan;
	    for (x = 0; x < out_width; x++) {
		for (c = 0; c < ((3) + (1)); c++)
		    *out_ptr++ = 255;
	    }
	}
    }
#endif

    /* initialize tables */
    VPComputeWarpTables();

    /* compute the intersection of the input image and the output image */
    /* filter width = 2.0 in input image space (triangle filter) */
    VPAffineImageOverlap(in_width, in_height, out_width, out_height,
			 warp_matrix, 2., full_overlap, part_overlap);

    /* compute the output image */
    out_ptr = out_image;
    out_scan_y = 0;
    denom = 1. / (warp_matrix[0][0] * warp_matrix[1][1] -
		  warp_matrix[0][1] * warp_matrix[1][0]);
    in_x_incr = warp_matrix[1][1]*denom;
    in_y_incr = -warp_matrix[1][0]*denom;
    if (in_x_incr < 0) {
	in_x_incr_int = (int)ceil(in_x_incr);
	in_x_incr_dlt = -1;
    } else {
	in_x_incr_int = (int)floor(in_x_incr);
	in_x_incr_dlt = 1;
    }
    if (in_y_incr < 0) {
	in_y_incr_int = (int)ceil(in_y_incr);
	in_y_incr_dlt = -1;
    } else {
	in_y_incr_int = (int)floor(in_y_incr);
	in_y_incr_dlt = 1;
    }
    xfrac_incr = FLTFRAC_TO_FIX31(in_x_incr - in_x_incr_int);
    yfrac_incr = FLTFRAC_TO_FIX31(in_y_incr - in_y_incr_int);
    for (region = 0; region < 9; region++) {
	/* check for empty region */
	if (part_overlap[region].miny >= out_height) {
	    break;
	}

	/* check if this region of the output image is unaffected by
	   the input image */
	if (part_overlap[region].x_top_lft >
	    part_overlap[region].x_top_rgt) {
	    c = (part_overlap[region].maxy - part_overlap[region].miny + 1) *
		out_bytes_per_scan;
	    memset(out_ptr, 0, c);
	    out_ptr += c;
	    out_scan_y += part_overlap[region].maxy -
			  part_overlap[region].miny + 1;
	    continue;
	}

	/* process scanlines of this region */
	scans_to_next_vertex = part_overlap[region].maxy -
			       part_overlap[region].miny + 1;
	x_lft_full = full_overlap[region].x_top_lft;
	x_rgt_full = full_overlap[region].x_top_rgt;
	x_lft_part = part_overlap[region].x_top_lft;
	x_rgt_part = part_overlap[region].x_top_rgt;
	if (x_lft_full > x_rgt_full)
	    no_full_pixels = 1;
	else
	    no_full_pixels = 0;
	ASSERT(scans_to_next_vertex > 0);
	ASSERT(out_scan_y == part_overlap[region].miny);
	while (scans_to_next_vertex > 0) {
	    /* compute the portions of the scanline which are zero
	       and which intersect the full and partially-full regions */
	    lft_zero_cnt = (int)floor(x_lft_part);
	    if (lft_zero_cnt < 0)
		lft_zero_cnt = 0;
	    else if (lft_zero_cnt > out_width)
		lft_zero_cnt = out_width;
	    if (no_full_pixels) {
		lft_edge_cnt = (int)ceil(x_rgt_part);
		if (lft_edge_cnt < 0)
		    lft_edge_cnt = 0;
		else if (lft_edge_cnt > out_width)
		    lft_edge_cnt = out_width;
		lft_edge_cnt -= lft_zero_cnt;
		if (lft_edge_cnt < 0)
		    lft_edge_cnt = 0;
		full_cnt = 0;
		rgt_edge_cnt = 0;
		rgt_zero_cnt = out_width - lft_zero_cnt - lft_edge_cnt;
	    } else {
		lft_edge_cnt = (int)ceil(x_lft_full);
		if (lft_edge_cnt < 0)
		    lft_edge_cnt = 0;
		else if (lft_edge_cnt > out_width)
		    lft_edge_cnt = out_width;
		lft_edge_cnt -= lft_zero_cnt;
		if (lft_edge_cnt < 0)
		    lft_edge_cnt = 0;
		full_cnt = (int)floor(x_rgt_full);
		if (full_cnt < 0)
		    full_cnt = 0;
		else if (full_cnt > out_width)
		    full_cnt = out_width;
		full_cnt -= lft_edge_cnt + lft_zero_cnt;
		if (full_cnt < 0)
		    full_cnt = 0;
		rgt_edge_cnt = (int)ceil(x_rgt_part);
		if (rgt_edge_cnt < 0)
		    rgt_edge_cnt = 0;
		else if (rgt_edge_cnt > out_width)
		    rgt_edge_cnt = out_width;
		rgt_edge_cnt -= full_cnt + lft_edge_cnt + lft_zero_cnt;
		if (rgt_edge_cnt < 0)
		    rgt_edge_cnt = 0;
		rgt_zero_cnt = out_width - lft_zero_cnt - lft_edge_cnt - 
		    	       full_cnt - rgt_edge_cnt;
	    }

	    /* reverse map the first left-edge output pixel coordinate into
	       the input image coordinate system */
	    in_x = ((lft_zero_cnt - warp_matrix[0][2]) * warp_matrix[1][1] -
		    (out_scan_y - warp_matrix[1][2])*warp_matrix[0][1])*denom;
	    in_y = (-(lft_zero_cnt - warp_matrix[0][2]) * warp_matrix[1][0] +
		    (out_scan_y - warp_matrix[1][2])*warp_matrix[0][0])*denom;
	    in_x_int = (int)floor(in_x);
	    in_y_int = (int)floor(in_y);
	    in_ptr = (RGBIntPixel *)(((char *)in_image + in_y_int *
				       in_bytes_per_scan)) + in_x_int;

	    /* compute the weight lookup table indices and increments */
	    xfrac = FLTFRAC_TO_FIX31(in_x - in_x_int);
	    yfrac = FLTFRAC_TO_FIX31(in_y - in_y_int);

	    /* zero out unaffected pixels on left edge of scan */
	    if (lft_zero_cnt > 0) {
		memset(out_ptr, 0, lft_zero_cnt * ((3) + (1))); 
		out_ptr += lft_zero_cnt * ((3) + (1));
	    }

	    /* process left edge case pixels */
	    for (x = lft_zero_cnt; x < lft_zero_cnt + lft_edge_cnt; x++) {
		wptr = VPBilirpWeight[FIX31_TO_WGTIND(yfrac)]
		    		     [FIX31_TO_WGTIND(xfrac)];
		
	r_acc = g_acc = b_acc = 0;
	opc_acc = 0;;
		if (in_x_int >= 0 && in_x_int < in_width) {
		    if (in_y_int >= 0 && in_y_int < in_height) {
			
	
		r_acc += (wptr[0]) * (in_ptr[0].rclrflt);
		g_acc += (wptr[0]) * (in_ptr[0].gclrflt);
		b_acc += (wptr[0]) * (in_ptr[0].bclrflt);
	opc_acc += (wptr[0]) * (in_ptr[0].opcflt);;
		    }
		    if (in_y_int+1 >= 0 && in_y_int+1 < in_height) {
			
	
		r_acc += (wptr[2]) * (in_ptr[in_width].rclrflt);
		g_acc += (wptr[2]) * (in_ptr[in_width].gclrflt);
		b_acc += (wptr[2]) * (in_ptr[in_width].bclrflt);
	opc_acc += (wptr[2]) * (in_ptr[in_width].opcflt);;
		    }
		}
		if (in_x_int+1 >= 0 && in_x_int+1 < in_width) {
		    if (in_y_int >= 0 && in_y_int < in_height) {
			
	
		r_acc += (wptr[1]) * (in_ptr[1].rclrflt);
		g_acc += (wptr[1]) * (in_ptr[1].gclrflt);
		b_acc += (wptr[1]) * (in_ptr[1].bclrflt);
	opc_acc += (wptr[1]) * (in_ptr[1].opcflt);;
		    }
		    if (in_y_int+1 >= 0 && in_y_int+1 < in_height) {
			
	
		r_acc += (wptr[3]) * (in_ptr[in_width + 1].rclrflt);
		g_acc += (wptr[3]) * (in_ptr[in_width + 1].gclrflt);
		b_acc += (wptr[3]) * (in_ptr[in_width + 1].bclrflt);
	opc_acc += (wptr[3]) * (in_ptr[in_width + 1].opcflt);;
		    }
		}
		
	    
	opc_acc_int = opc_acc * (float)255.;
	if (opc_acc_int > 255)
	    opc_acc_int = 255;
	((out_ptr)[0]) = opc_acc_int;
	    
		r_acc_int = r_acc;
		if (r_acc_int > 255)
		    r_acc_int = 255;
		((out_ptr)[1+2]) = r_acc_int;
		g_acc_int = g_acc;
		if (g_acc_int > 255)
		    g_acc_int = 255;
		((out_ptr)[1+1]) = g_acc_int;
		b_acc_int = b_acc;
		if (b_acc_int > 255)
		    b_acc_int = 255;
		((out_ptr)[1+0]) = b_acc_int;;
		out_ptr += ((3) + (1));
		xfrac += xfrac_incr;
		yfrac += yfrac_incr;
		if (xfrac < 0) {
		    xfrac &= 0x7fffffff;
		    in_x_int += in_x_incr_int + in_x_incr_dlt;
		    in_ptr += in_x_incr_int + in_x_incr_dlt;
		} else {
		    in_x_int += in_x_incr_int;
		    in_ptr += in_x_incr_int;
		}
		if (yfrac < 0) {
		    yfrac &= 0x7fffffff;
		    in_y_int += in_y_incr_int + in_y_incr_dlt;
		    in_ptr += in_width * (in_y_incr_int + in_y_incr_dlt);
		} else {
		    in_y_int += in_y_incr_int;
		    in_ptr += in_width * in_y_incr_int;
		}
	    }

	    /* process output pixels affected by four input pixels */
	    for (x = lft_zero_cnt + lft_edge_cnt;
		 x < lft_zero_cnt + lft_edge_cnt + full_cnt; x++) {
		ASSERT(in_x_int >= 0 && in_x_int < in_width-1);
		ASSERT(in_y_int >= 0 && in_y_int < in_height-1);
		ASSERT((RGBIntPixel *)(((char *)in_image + in_y_int *
				in_bytes_per_scan)) + in_x_int == in_ptr);
		wptr = VPBilirpWeight[FIX31_TO_WGTIND(yfrac)]
				     [FIX31_TO_WGTIND(xfrac)];
		
	
		r_acc = (wptr[0]) * (in_ptr[0].rclrflt) +
			(wptr[2]) * (in_ptr[in_width].rclrflt) +	
			(wptr[1]) * (in_ptr[1].rclrflt) +
			(wptr[3]) * (in_ptr[in_width+1].rclrflt);
		g_acc = (wptr[0]) * (in_ptr[0].gclrflt) +
			(wptr[2]) * (in_ptr[in_width].gclrflt) +	
			(wptr[1]) * (in_ptr[1].gclrflt) +
			(wptr[3]) * (in_ptr[in_width+1].gclrflt);
		b_acc = (wptr[0]) * (in_ptr[0].bclrflt) +
			(wptr[2]) * (in_ptr[in_width].bclrflt) +	
			(wptr[1]) * (in_ptr[1].bclrflt) +
			(wptr[3]) * (in_ptr[in_width+1].bclrflt);
	
	opc_acc = (wptr[0]) * (in_ptr[0].opcflt) +
		  (wptr[2]) * (in_ptr[in_width].opcflt) +	
		  (wptr[1]) * (in_ptr[1].opcflt) +
		  (wptr[3]) * (in_ptr[in_width+1].opcflt);;
		
	    
	opc_acc_int = opc_acc * (float)255.;
	if (opc_acc_int > 255)
	    opc_acc_int = 255;
	((out_ptr)[0]) = opc_acc_int;
	    
		r_acc_int = r_acc;
		if (r_acc_int > 255)
		    r_acc_int = 255;
		((out_ptr)[1+2]) = r_acc_int;
		g_acc_int = g_acc;
		if (g_acc_int > 255)
		    g_acc_int = 255;
		((out_ptr)[1+1]) = g_acc_int;
		b_acc_int = b_acc;
		if (b_acc_int > 255)
		    b_acc_int = 255;
		((out_ptr)[1+0]) = b_acc_int;;
		out_ptr += ((3) + (1));
		xfrac += xfrac_incr;
		yfrac += yfrac_incr;
		if (xfrac < 0) {
		    xfrac &= 0x7fffffff;
		    in_x_int += in_x_incr_int + in_x_incr_dlt;
		    in_ptr += in_x_incr_int + in_x_incr_dlt;
		} else {
		    in_x_int += in_x_incr_int;
		    in_ptr += in_x_incr_int;
		}
		if (yfrac < 0) {
		    yfrac &= 0x7fffffff;
		    in_y_int += in_y_incr_int + in_y_incr_dlt;
		    in_ptr += in_width * (in_y_incr_int + in_y_incr_dlt);
		} else {
		    in_y_int += in_y_incr_int;
		    in_ptr += in_width * in_y_incr_int;
		}
	    }

	    /* process right edge case pixels */
	    for (x = lft_zero_cnt + lft_edge_cnt + full_cnt;
		 x < lft_zero_cnt + lft_edge_cnt + full_cnt + rgt_edge_cnt;
		 x++) {
		wptr = VPBilirpWeight[FIX31_TO_WGTIND(yfrac)]
				     [FIX31_TO_WGTIND(xfrac)];
		
	r_acc = g_acc = b_acc = 0;
	opc_acc = 0;;
		if (in_x_int >= 0 && in_x_int < in_width) {
		    if (in_y_int >= 0 && in_y_int < in_height) {
			
	
		r_acc += (wptr[0]) * (in_ptr[0].rclrflt);
		g_acc += (wptr[0]) * (in_ptr[0].gclrflt);
		b_acc += (wptr[0]) * (in_ptr[0].bclrflt);
	opc_acc += (wptr[0]) * (in_ptr[0].opcflt);;
		    }
		    if (in_y_int+1 >= 0 && in_y_int+1 < in_height) {
			
	
		r_acc += (wptr[2]) * (in_ptr[in_width].rclrflt);
		g_acc += (wptr[2]) * (in_ptr[in_width].gclrflt);
		b_acc += (wptr[2]) * (in_ptr[in_width].bclrflt);
	opc_acc += (wptr[2]) * (in_ptr[in_width].opcflt);;
		    }
		}
		if (in_x_int+1 >= 0 && in_x_int+1 < in_width) {
		    if (in_y_int >= 0 && in_y_int < in_height) {
			
	
		r_acc += (wptr[1]) * (in_ptr[1].rclrflt);
		g_acc += (wptr[1]) * (in_ptr[1].gclrflt);
		b_acc += (wptr[1]) * (in_ptr[1].bclrflt);
	opc_acc += (wptr[1]) * (in_ptr[1].opcflt);;
		    }
		    if (in_y_int+1 >= 0 && in_y_int+1 < in_height) {
			
	
		r_acc += (wptr[3]) * (in_ptr[in_width + 1].rclrflt);
		g_acc += (wptr[3]) * (in_ptr[in_width + 1].gclrflt);
		b_acc += (wptr[3]) * (in_ptr[in_width + 1].bclrflt);
	opc_acc += (wptr[3]) * (in_ptr[in_width + 1].opcflt);;
		    }
		}
		
	    
	opc_acc_int = opc_acc * (float)255.;
	if (opc_acc_int > 255)
	    opc_acc_int = 255;
	((out_ptr)[0]) = opc_acc_int;
	    
		r_acc_int = r_acc;
		if (r_acc_int > 255)
		    r_acc_int = 255;
		((out_ptr)[1+2]) = r_acc_int;
		g_acc_int = g_acc;
		if (g_acc_int > 255)
		    g_acc_int = 255;
		((out_ptr)[1+1]) = g_acc_int;
		b_acc_int = b_acc;
		if (b_acc_int > 255)
		    b_acc_int = 255;
		((out_ptr)[1+0]) = b_acc_int;;
		out_ptr += ((3) + (1));
		xfrac += xfrac_incr;
		yfrac += yfrac_incr;
		if (xfrac < 0) {
		    xfrac &= 0x7fffffff;
		    in_x_int += in_x_incr_int + in_x_incr_dlt;
		    in_ptr += in_x_incr_int + in_x_incr_dlt;
		} else {
		    in_x_int += in_x_incr_int;
		    in_ptr += in_x_incr_int;
		}
		if (yfrac < 0) {
		    yfrac &= 0x7fffffff;
		    in_y_int += in_y_incr_int + in_y_incr_dlt;
		    in_ptr += in_width * (in_y_incr_int + in_y_incr_dlt);
		} else {
		    in_y_int += in_y_incr_int;
		    in_ptr += in_width * in_y_incr_int;
		}
	    }

	    /* zero out unaffected pixels on right edge of scan */
	    if (rgt_zero_cnt > 0) {
		memset(out_ptr, 0, rgt_zero_cnt * ((3) + (1)));
		out_ptr += rgt_zero_cnt * ((3) + (1));
	    }

	    /* go on to next scan */
	    scans_to_next_vertex--;
	    out_scan_y++;
	    out_ptr += out_bytes_per_scan - out_width * ((3) + (1));
	    x_lft_full += full_overlap[region].x_incr_lft;
	    x_rgt_full += full_overlap[region].x_incr_rgt;
	    x_lft_part += part_overlap[region].x_incr_lft;
	    x_rgt_part += part_overlap[region].x_incr_rgt;
	} /* next scanline in region */
    } /* next region */
    ASSERT(out_scan_y == out_height);
}
