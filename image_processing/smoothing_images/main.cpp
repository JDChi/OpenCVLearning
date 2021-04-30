#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

Mat src , dst;
char window_name[] = "Smothing Demo";

int display_caption(const char* caption);
int display_dst(int delay);
void myBlur(Mat src , Mat dst , int i);
void myGaussianBlur(Mat src , Mat dst , int i);
void myMedianBlur(Mat src , Mat dst , int i);
void myBilateralFilter(Mat src , Mat dst , int i);

/*
 blur 均值滤波
 给出一个滤波模板kernel，该模板将覆盖像素点周围的其他邻域像素点，去掉像素本身，将其邻域像素点相加然后取平均值即为该像素点的新的像素值
 https://blog.csdn.net/keith_bb/article/details/54382426?spm=1001.2014.3001.5501
 
 medianBlur 中值滤波
 基本思想是用像素点邻域灰度值的中值来代替该像素点的灰度值，让周围的像素值接近真实的值从而消除孤立的噪声点。
 https://blog.csdn.net/keith_bb/article/details/54426920?spm=1001.2014.3001.5501
 
 GaussianBlur 高斯滤波
 用一个模板（或称卷积、掩模）扫描图像中的每一个像素，用模板确定的邻域内像素的加权平均灰度值去替代模板中心像素点的值
 https://blog.csdn.net/keith_bb/article/details/54412493?spm=1001.2014.3001.5501
 
 bilateralFilter 双边滤波
 结合图像的空间邻近度和像素值相似度的一种折衷处理，同时考虑空间与信息和灰度相似性，达到保边去噪的目的
 https://blog.csdn.net/guyuealian/article/details/82660826
 */
int main(int argc, const char * argv[]) {
    namedWindow(window_name);
    const char* filename = "/Users/wecut1260/Desktop/hello.png";
    src = imread(samples::findFile(filename) , IMREAD_COLOR);
    if (src.empty()) {
        printf("Error opening image\n");
        return EXIT_FAILURE;
    }
    
    if(display_caption("Original Image") != 0)
        return 0;
    
    dst = src.clone();
    
    if(display_dst(DELAY_CAPTION) != 0)
        return 0;
    
    if(display_caption("Homogeneous Blur") != 0)
        return 0;
    
    for (int i= 1; i < MAX_KERNEL_LENGTH; i = i + 2) {
        myBlur(src , dst , i);
        if(display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    
    if(display_caption("Gaussian Blur") != 0)
        return 0;
    
    for(int i = 1 ; i < MAX_KERNEL_LENGTH ; i = i + 2)
    {
        myGaussianBlur(src , dst , i);
        if(display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    
    if(display_caption("Median Blur") != 0)
        return 0;
    
    for(int i = 1 ; i < MAX_KERNEL_LENGTH ; i = i + 2)
    {
        myMedianBlur(src , dst , i);
        if(display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    
    if(display_caption("Bilateral Blur") != 0)
        return 0;
    
    for(int i = 1 ; i < MAX_KERNEL_LENGTH ; i = i + 2)
    {
        myBilateralFilter(src , dst , i);
        if(display_dst(DELAY_BLUR) != 0)
            return 0;
    }
    
    return 0;
}

/*
 - Size ksize: 滤波模板kernel的尺寸，一般使用Size(w, h)来指定，如Size(3,3)
 - Point anchor=Point(-1, -1): 字面意思是锚点，也就是处理的像素位于kernel的什么位置，默认值为(-1, -1)即位于kernel中心点，如果没有特殊需要则不需要更改
 */
void myBlur(Mat src , Mat dst , int i){
    blur(src, dst, Size(i , i) , Point(-1 , -1));
}

/*
 - Size ksize: 高斯内核大小，这个尺寸与前面两个滤波kernel尺寸不同，ksize.width和ksize.height可以不相同但是这两个值必须为正奇数，如果这两个值为0，他们的值将由sigma计算。
 - double sigmaX: 高斯核函数在X方向上的标准偏差
 - double sigmaY: 高斯核函数在Y方向上的标准偏差，如果sigmaY是0，则函数会自动将sigmaY的值设置为与sigmaX相同的值，如果sigmaX和sigmaY都是0，这两个值将由ksize.width和ksize.height计算而来。具体可以参考getGaussianKernel()函数查看具体细节。建议将size、sigmaX和sigmaY都指定出来。

 */
void myGaussianBlur(Mat src , Mat dst , int i){
    GaussianBlur(src, dst, Size(i , i), 0 , 0);
}

/*
 - int ksize: 滤波模板的尺寸大小，必须是大于1的奇数，如3、5、7……
 */
void myMedianBlur(Mat src , Mat dst , int i){
    medianBlur(src, dst, i);
}

/*
 - int d: 表示在过滤过程中每个像素邻域的直径范围。如果这个值是非正数，则函数会从第五个参数sigmaSpace计算该值。
 - double sigmaColor: 颜色空间过滤器的sigma值，这个参数的值越大，表明该像素邻域内有越宽广的颜色会被混合到一起，产生较大的半相等颜色区域。
 - double sigmaSpace: 坐标空间中滤波器的sigma值，如果该值较大，则意味着颜色相近的较远的像素将相互影响，从而使更大的区域中足够相似的颜色获取相同的颜色。当d>0时，d指定了邻域大小且与sigmaSpace五官，否则d正比于sigmaSpace.
 */
void myBilateralFilter(Mat src , Mat dst , int i){
    bilateralFilter(src, dst, i, i*2, i/2);
}

int display_caption(const char* caption)
{
    dst = Mat::zeros(src.size(), src.type());
    putText(dst, caption, Point(src.cols/4, src.rows/2), FONT_HERSHEY_COMPLEX, 1 , Scalar(255,255,255));
    return  display_dst(DELAY_CAPTION);
}

int display_dst(int delay)
{
    imshow(window_name, dst);
    int c = waitKey(delay);
    if(c >= 0)
        return -1;
    return 0;
}
