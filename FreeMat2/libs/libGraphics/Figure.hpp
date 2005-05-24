#ifndef __Figure_hpp__
#define __Figure_hpp__

#include "XPWindow.hpp"
#include "PrintableWidget.hpp"
#include <string>

namespace FreeMat {
  
  typedef enum {fignone, figplot, figscimg, figgui, fig3plot} figType;
  
  class Figure : public XPWindow {
  public:
    Figure(int fignum);
    ~Figure();
    figType getType() {return m_type;};
    void SetFigureChild(PrintableWidget *widget, figType w_type);
    PrintableWidget* GetChildWidget();
    void Copy();
  private:
    int m_num;
    figType m_type;
    PrintableWidget *m_wid;
  };

  Figure* GetCurrentFig();
}
#endif
