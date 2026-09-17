#include <array>

class arrayTranslator {
    private:
    std::array<std::array<int, 15>, 16> horz;
    std::array<std::array<int, 16>, 15> vert;
    std::array<std::array<char,61>, 31> maze;



    public:
    


    arrayTranslator(const std::array<std::array<int, 15>, 16>&h, const std::array<std::array<int, 16>, 15>&v) : horz(h), vert(v) {
        
    }

    std::array<std::array<char,61>, 31> translate() {

        for (int i = 0; i < 31; i++) {
            for (int j = 0; j < 61; j++) {
                
                maze[i][j] = ' ';
                
            }
        }
        
        
        //This fills in the nodes.
        for (int a = 0; a < 31; a += 2) {
            for (int b = 0; b < 61; b++) {
                if (((b+1)%4 ==1)) {
                    maze[a][b] = '+';
                }
            }
        }
        

        //filling in the rows:

        
        for (int hy=0;hy<15;hy++) {
            for (int hx=0;hx<16;hx++) {
                if(horz[hx][hy]==0) {
                    maze[hx*2][hy*4 + 1] = ' ';
                    maze[hx*2][hy*4 + 2] = ' ';
                    maze[hx*2][hy*4 + 3] = ' ';
                }
                if(horz[hx][hy]==1) {
                    maze[hx*2][hy*4 + 1] = '-';
                    maze[hx*2][hy*4 + 2] = '-';
                    maze[hx*2][hy*4 + 3] = '-';
                }
            }
        }

        //filling in columns
        for (int cy=0;cy<16;cy++) {
            for (int cx=0;cx<15;cx++) {
                if(vert[cx][cy]==0) {
                    maze[(cx+1)*2-1][cy*4] = ' ';
                }
                if(vert[cx][cy]==1) {
                    maze[(cx+1)*2-1][cy*4] = '|';
                }
            }
        }
            
        return maze;

    }
    



};