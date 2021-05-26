#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
       
int main(int argc, char *argv[]) 
{
    int color_max;
    int width, height;
    char p, s;
 
    //load info about image
    FILE *input_file = fopen(argv[1], "rb");
    fscanf(input_file, "%c%c\n%d\n%d\n%d\n", &p, &s, &width, &height, &color_max);

    //algorithm
    uint8_t *aux;
    uint8_t *new_line = malloc(sizeof(new_line) * width * 3);
    uint8_t *top_line = malloc(sizeof(top_line) * width * 3);
    uint8_t *middle_line = malloc(sizeof(top_line) * width * 3);
    uint8_t *bottom_line = malloc(sizeof(top_line) * width * 3);
    //read first line;
    fread(&bottom_line[0], 3, width, input_file);
    //write info into new image
    FILE *output_file = fopen("output.ppm", "wb");
    fprintf(output_file, "P6\n%d\n%d\n255\n", width, height);
    
    short int r = 0;
    short int g = 0;
    short int b = 0;
    uint8_t count = 0;
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;
    for(int i = 0; i < height; i++){
        aux = top_line;
        top_line = middle_line;
        middle_line = bottom_line;
        bottom_line = aux;
        fread(&bottom_line[0], 3, width, input_file);
        // sharpen
        for(int pxl = 0; pxl < width*3; pxl+=3){
            r = 0; g = 0; b = 0;
            if(pxl == 0 || pxl+3 == width*3 || i == 0 || i+1 == height){
                new_line[pxl] = middle_line[pxl];
                new_line[pxl + 1] = middle_line[pxl + 1];
                new_line[pxl + 2] = middle_line[pxl + 2];
                count = (2126*middle_line[pxl] + 7152*middle_line[pxl + 1] + 722*middle_line[pxl + 2] + 5000)/10000;
            }
            else {
                r -= top_line[pxl] + bottom_line[pxl];
                g -= top_line[pxl + 1] + bottom_line[pxl + 1];
                b -= top_line[pxl + 2] + bottom_line[pxl + 2];
                r += middle_line[pxl] * 5 - middle_line[pxl - 3] - middle_line[pxl + 3];
                g += middle_line[pxl + 1] * 5 - middle_line[pxl - 2] - middle_line[pxl + 4];
                b += middle_line[pxl + 2] * 5 - middle_line[pxl - 1] - middle_line[pxl + 5];

                r = (r < 0 ? 0 : r);
                g = (g < 0 ? 0 : g);
                b = (b < 0 ? 0 : b);
                r = (r < 255 ? r : 255);
                g = (g < 255 ? g : 255);
                b = (b < 255 ? b : 255);

                new_line[pxl] = r;
                new_line[pxl + 1] = g;
                new_line[pxl + 2] = b;
                count = (2126*r + 7152*g + 722*b + 5000)/10000;
            }
             // make output.txt
            if ((count > 152) && (count < 204)){
                c4++;
                continue;
            }
            if (count > 203){
                c5++;
                continue;
            }
            if ((count < 102) && (count > 50)){
                c2++;
                continue;
            }
            if (count < 51){
                c1++;
                continue;
            }
            c3++;
        }
        // write middle line
        fwrite(&new_line[0], 3, width, output_file);
    }
    fclose(input_file);
    fclose(output_file);
    //make output.txt
    FILE *txt = fopen("output.txt", "wb");
    fprintf(txt, "%d %d %d %d %d", c1, c2, c3, c4, c5);
    fclose(txt);

    //free
    free(new_line);
    free(top_line);
    free(middle_line);
    free(bottom_line);
 
    return 0;
}
