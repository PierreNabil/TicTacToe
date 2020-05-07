#include <ccc_win.h>
#include <vector>

//Function Declaration
/**
  corrects the position of a point to one of the 9 correct positions on the board
  @param pnt input point detected from the user
  @return true_pnt corrected position of the point
*/
int correct_pos(Point& pnt);

/**
  clears the screen and redraws the board
*/
void game_start();


//Classes
class board //Class to keep track of the game
{
public:
  board();

  void reset();

  void play(Point P,char c);

  bool check(Point& P) const;
  bool check_win() const;

private:
  char Grid[3][3];
};
board::board()
{
  reset();
}
void board::reset()
{
  for(int i=0; i<3; i++)
    for(int j=0; j<3; j++)
      Grid[i][j] = 0;
}
void board::play(Point P,char c)
{
  char pos = correct_pos(P);
  if (pos==1) Grid[0][0] = c;
  else if (pos==2) Grid[0][1] = c;
  else if (pos==3) Grid[0][2] = c;
  else if (pos==4) Grid[1][0] = c;
  else if (pos==5) Grid[1][1] = c;
  else if (pos==6) Grid[1][2] = c;
  else if (pos==7) Grid[2][0] = c;
  else if (pos==8) Grid[2][1] = c;
  else if (pos==9) Grid[2][2] = c;

}
bool board::check(Point& P) const
{
  bool ans;
  char pos = correct_pos(P);

  if (pos==1) ans = Grid[0][0];
  else if (pos==2) ans = Grid[0][1];
  else if (pos==3) ans = Grid[0][2];
  else if (pos==4) ans = Grid[1][0];
  else if (pos==5) ans = Grid[1][1];
  else if (pos==6) ans = Grid[1][2];
  else if (pos==7) ans = Grid[2][0];
  else if (pos==8) ans = Grid[2][1];
  else if (pos==9) ans = Grid[2][2];

  return ans;
}
bool board::check_win() const
{
  bool ans =
        (Grid[0][0]==Grid[0][1] && Grid[0][0]==Grid[0][2] && Grid[0][0]!=false)//three columns
      ||(Grid[1][0]==Grid[1][1] && Grid[1][0]==Grid[1][2] && Grid[1][0]!=false)
      ||(Grid[2][0]==Grid[2][1] && Grid[2][0]==Grid[2][2] && Grid[2][0]!=false)
      ||(Grid[0][0]==Grid[1][0] && Grid[0][0]==Grid[2][0] && Grid[0][0]!=false)//three rows
      ||(Grid[0][1]==Grid[1][1] && Grid[0][1]==Grid[2][1] && Grid[0][1]!=false)
      ||(Grid[0][2]==Grid[1][2] && Grid[0][2]==Grid[2][2] && Grid[0][2]!=false)
      ||(Grid[0][0]==Grid[1][1] && Grid[0][0]==Grid[2][2] && Grid[0][0]!=false)//two diagonals
      ||(Grid[2][0]==Grid[1][1] && Grid[2][0]==Grid[0][2] && Grid[2][0]!=false);
  return ans;
}

class nought //Nought (O) Class
{
public:
  nought();

  void draw(Point P);
  void Turn(board& B);

private:
  float r;
  Circle shape;
};
nought::nought()
{
  r = 2;
  shape = Circle(Point(0,0),r);
}
void nought::draw(Point P)
{
  correct_pos(P);
  shape.move(P.get_x(),P.get_y());
  cwin << shape;
  shape.move(-P.get_x(),-P.get_y());
}
void nought::Turn(board& B)
{
  Point turn;
  while(1)
  {
    turn = cwin.get_mouse("Nought's turn:");
    if (B.check(turn)==false) break;
  }
  B.play(turn,'O');
  draw(turn);
}

class cross //Cross (X) Class
{
  public:
  cross();

  void draw(Point P);
  void move_cross(double dx, double dy);
  void Turn(board& B);

private:
  Line L1;
  Line L2;
};
cross::cross()
{
  L1 = Line(Point(1.414,1.414),Point(-1.414,-1.414));
  L2 = Line(Point(1.414,-1.414),Point(-1.414,1.414));
}
void cross::move_cross(double dx, double dy)
{
  L1.move(dx,dy);
  L2.move(dx,dy);
}
void cross::draw(Point P)
{
  correct_pos(P);
  move_cross(P.get_x(),P.get_y());
  cwin << L1 <<L2;
  move_cross(-P.get_x(),-P.get_y());
}
void cross::Turn(board& B)
{
  Point turn;
  while(1)
  {
    turn = cwin.get_mouse("Cross's turn:");
    if (B.check(turn)==false) break;
  }
  B.play(turn,'X');
  draw(turn);
}


//Main
void ccc_win_main()
{
  board B;
  cross X;
  nought O;
  int turn_no;
  bool win = false;

  while (1) //loops once per game
  {
    game_start();
    B.reset();
    turn_no = 0;

    while(1) //loops once per 2 turns
    {
      X.Turn(B);
      if (B.check_win()) break;
      turn_no++;
      if (turn_no>=8) break;

      O.Turn(B);
      if (B.check_win()) break;
      turn_no++;
    }
    if (B.check_win()==false) cwin.get_mouse("Draw! \nGame Over");
    else if (turn_no%2==0) cwin.get_mouse("Crosses wins! \nGame Over");
    else cwin.get_mouse("Noughts wins! \nGame Over");
  }
}


//Function Definitions
int correct_pos(Point& pnt)
{
  double x = pnt.get_x(), y = pnt.get_y();
  char pos;     // numbers arranged like the std number layout of phones, calculators...etc.
  Point true_pnt;
  //3 right columns
  if(x>3.33 && y>3.33){true_pnt = Point(6.66,6.66); pos = 9;}
  else if(x>3.33 && y<-3.33){true_pnt = Point(6.66,-6.66); pos = 3;}
  else if(x>3.33 && y<3.33 && y>-3.33){true_pnt = Point(6.66,0); pos = 6;}
  //3 left columns
  else if(x<-3.33 && y>3.33){true_pnt = Point(-6.66,6.66); pos = 7;}
  else if(x<-3.33 && y<-3.33){true_pnt = Point(-6.66,-6.66); pos = 1;}
  else if(x<-3.33 && y<3.33 && y>-3.33){true_pnt = Point(-6.66,0); pos = 4;}
  //3 middle columns
  else if(x<3.33 && x>-3.33 && y>3.33){true_pnt = Point(0,6.66); pos = 8;}
  else if(x<3.33 && x>-3.33 && y<-3.33){true_pnt = Point(0,-6.66); pos = 2;}
  else {true_pnt = Point(0,0); pos = 5;}

  pnt = true_pnt;
  return pos;
}

void game_start()
{
  cwin.clear();
  cwin << Line(Point(-3.33,10),Point(-3.33,-10))
      <<Line(Point(3.33,10),Point(3.33,-10))
      <<Line(Point(10,3.33),Point(-10,3.33))
      <<Line(Point(10,-3.33),Point(-10,-3.33));
}

