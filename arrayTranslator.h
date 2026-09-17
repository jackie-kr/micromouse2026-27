#include <array>

class arrayTranslator {
    private:
    std::array<std::array<int, 16>, 17> horz;
    std::array<std::array<int, 17>, 16> vert;
    std::array<std::array<char,65>, 33> maze;



    public:
    


    arrayTranslator(const std::array<std::array<int, 16>, 17>&h, const std::array<std::array<int, 17>, 16>&v) : horz(h), vert(v) {
        
    }

    std::array<std::array<char,65>, 33> translate() {

        for (int i = 0; i < 33; i++) {
            for (int j = 0; j < 65; j++) {
                
                maze[i][j] = ' ';
                
            }
        }
        
        
        //This fills in the nodes.
        for (int a = 0; a < 33; a += 2) {
            for (int b = 0; b < 65; b++) {
                if (((b+1)%4 ==1)) {
                    maze[a][b] = '+';
                }
            }
        }
        

        //filling in the rows:

        
        for (int hy=0;hy<16;hy++) {
            for (int hx=0;hx<17;hx++) {
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
        for (int cy=0;cy<17;cy++) {
            for (int cx=0;cx<16;cx++) {
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