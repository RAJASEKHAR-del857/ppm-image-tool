#include <iostream>
#include "libppm.h"
#include <string>
#include <cstdlib>
#include <cstdint>
#include <chrono>

using namespace std;

uint8_t clamp(int value) {
    if(value < 0) return 0;
    if(value > 255) return 255;
    return (uint8_t)value;
}

struct image_t* allocate_image(int height, int width) {
    struct image_t* img = (struct image_t*)malloc(sizeof(struct image_t));
    img->height = height;
    img->width = width;

    img->image_pixels = (uint8_t***)malloc(height * sizeof(uint8_t**));
    for (int i = 0; i < height; i++) {
        img->image_pixels[i] = (uint8_t**)malloc(width * sizeof(uint8_t*));
        for (int j = 0; j < width; j++) {
            img->image_pixels[i][j] = (uint8_t*)malloc(3 * sizeof(uint8_t));

            // Optional: zero-initialize
            img->image_pixels[i][j][0] = 0;
            img->image_pixels[i][j][1] = 0;
            img->image_pixels[i][j][2] = 0;
        }
    }

    return img;
}

void free_image(struct image_t* img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            free(img->image_pixels[i][j]);
        }
        free(img->image_pixels[i]);
    }
    free(img->image_pixels);
    free(img);
}

struct image_t* S1_smoothen(struct image_t *input_image)
{
    
    struct image_t * duplicate_image = allocate_image(input_image->height, input_image->width);
    for(int i=0;i<input_image->height;i++) {
        for(int j=0;j<input_image->width;j++) {
            for(int k=0;k<3;k++) {
                int sum=0, count=0;
                for(int di=-1;di<=1;di++) {
                    for(int dj=-1;dj<=1;dj++) {
                        if(i+di >=0 && i+di <input_image->height && j+dj >= 0 && j+dj < input_image->width){
                            sum += static_cast<int>(input_image->image_pixels[i+di][j+dj][k]);
                            count++;
                        }
                    }
                }
                if(count==0) count=1;
                duplicate_image->image_pixels[i][j][k] = static_cast<uint8_t>(sum/count);
            }
        }
    }
    return duplicate_image;
}
struct image_t* S2_find_details(struct image_t *input_image, struct image_t *smoothened_image)
{
    struct image_t* detailed_image = allocate_image(input_image->height, input_image->width);
   
    for(int i=0;i<input_image->height; i++) {
        for(int j=0;j<input_image->width;j++) {
            for(int c=0;c<3;c++) {
            detailed_image->image_pixels[i][j][c] = clamp(input_image->image_pixels[i][j][c] - smoothened_image->image_pixels[i][j][c]);
        }
    }
    }
    return detailed_image;
}

struct image_t* S3_sharpen(struct image_t *input_image, struct image_t *details_image)
{
    struct image_t* sharpened_image = allocate_image(input_image->height, input_image->width);
    for(int i=0;i<input_image->height; i++) {
        for(int j=0;j<input_image->width;j++) {
            for(int c=0;c<3;c++) {
            sharpened_image->image_pixels[i][j][c] = clamp(input_image->image_pixels[i][j][c] + details_image->image_pixels[i][j][c]);
        }
    }
    }
    return sharpened_image; 
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        cout << "usage: ./a.out <path-to-original-image> <path-to-transformed-image>\n\n";
        exit(0);
    }
    
    const auto start_read = std::chrono::steady_clock::now();
    struct image_t *input_image = read_ppm_file(argv[1]);
    const auto finish_read = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds_read = finish_read - start_read;
    cout << "For read_file , Time Taken is " << elapsed_seconds_read.count() << "seconds \n";
    
    const auto start1 = std::chrono::steady_clock::now();
    struct image_t *smoothened_image = S1_smoothen(input_image);
    const auto finish1 = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds1 = finish1 - start1;
    cout << "For s1_smoothen, Time Taken is " << elapsed_seconds1.count() << "seconds \n";
    
    const auto start2 = std::chrono::steady_clock::now();
    struct image_t *details_image = S2_find_details(input_image, smoothened_image);
    const auto finish2 = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds2 = finish2 - start2;
    cout << "For S2_find_details , Time Taken is " << elapsed_seconds2.count() << "seconds \n";
    
     const auto start3 = std::chrono::steady_clock::now();
    struct image_t *sharpened_image = S3_sharpen(input_image, details_image);
     const auto finish3 = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds3 = finish3 - start3;
    cout << "For S3_Sharpen , Time Taken is " << elapsed_seconds3.count() << "seconds \n";
    

    const auto start_write = std::chrono::steady_clock::now();
    write_ppm_file(argv[2], sharpened_image);
    const auto finish_write = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds_write = finish_write - start_write;
    cout << "For write_file , Time Taken is " << elapsed_seconds_write.count() << "seconds \n";
    
    /*
     This template function allows you to measure and print the time taken by any
	function, such as S1_smoothen(), S2_find_details(), S3_sharpen(), or others.

 	It accepts:
	- A label (string) to print during output
	- A function to execute
	- Any number of arguments to forward to that function

	Returns:
	- The result of the executed function (with full type preservation)

	FUNCTION:
	
	template<typename Func, typename... Args>
	auto measure_time(const std::string& label, Func func, Args&&... args)
    -> decltype(func(std::forward<Args>(args)...)) {
    
    auto start = std::chrono::steady_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();
    
    std::chrono::duration<double> elapsed = end - start;
    std::cout << label << " Time: " << elapsed.count() << " seconds\n";
    
    return result;
}
*/

    free_image(input_image);
    free_image(smoothened_image);
    free_image(details_image);
    free_image(sharpened_image);
    
    return 0;
}
