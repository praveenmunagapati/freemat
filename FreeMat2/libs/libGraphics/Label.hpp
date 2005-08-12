#ifndef __Label_hpp__
#define __Label_hpp__

#include "QPWidget.hpp"
#include "Point2D.hpp"

namespace FreeMat {
  class Box {
  public:
    int width;
    int height;
    int depth;
  };

  class FormulaTree {
    FormulaTree* m_supertree;
    FormulaTree* m_subtree;
    FormulaTree* m_nexttree;
    QString m_text;
    int m_size;
    int m_ascent;
  public:
    FormulaTree(QString text, FormulaTree* supertree, 
		FormulaTree* subtree, FormulaTree* nexttree);
    void PrintMe();
    Box GetBox();
    void SizeTree(int size, int ascent);
    void Render(QPainter& gc, Point2D& pos);
  };

  class TexLabel {
    std::string m_rawtext;
    QString m_processed_text; // (unicode) character stream
    std::vector<int> m_sizes; // size in points of each character
    std::vector<int> m_xpos_list;  // x position to draw text
    std::vector<int> m_ypos_list;  // y position to draw text
    QString m_output_text;
    int m_cp;
    int m_xpos;
    int m_ypos;
    int m_size;
    int m_supersub_level;
    FormulaTree *m_tree;
  public:
    TexLabel(std::string text);
    Point2D BoundingBox();
    void Substitute(QString ecode, QChar rcode);
    void DoSubstitutions();
    void Render(QPainter& gc, Point2D pos);
    void CompileRawText();
    int GetCurrentSize();
    int GetCurrentYPos();
    int GetCurrentWidth(QChar a);
    void Stringify();
  };

  class Label : public QPWidget {
  private:
    std::string m_text;
    QString m_processed_text;
    char m_orientation;
  public:
    Label(QWidget* parent, std::string text, char orient);
    void ProcessText();
    virtual ~Label();
    void DrawMe(QPainter& gc);
  };
}

#endif
