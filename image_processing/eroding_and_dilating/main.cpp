#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src , erosion_dst , dilation_dst;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

void Erosion(int , void*);
void Dilation(int , void*);

int main(int argc , char** argv)
{
    CommandLineParser parser(argc , argv , "{@input | /Users/wecut1260/Desktop/hello.png}");
    src = imread(samples::findFile(parser.get<String>("@input")) , IMREAD_COLOR);
    
    if(src.empty())
    {
        cout << "error image" << endl;
        return -1;
    }
    
    namedWindow("Erosion Demo");
    namedWindow("Dilation Demo");
    moveWindow("Dilation", src.cols, 0);
    
    // 创建轨迹条
    createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo", &erosion_elem, max_elem , Erosion);
    
    createTrackbar("Kernel size:\n 2n + 1", "Erosion Demo", &erosion_size, max_kernel_size , Erosion);
    
    createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo", &dilation_elem, max_elem , Dilation);
    
    createTrackbar("Kernel size:\n 2n + 1", "Dilation Demo", &dilation_size, max_kernel_size , Dilation);
    
    Erosion(0, 0);
    Dilation(0, 0);
    
    waitKey(0);
    return 0;
}

/*
 侵蚀
 
 侵蚀的基本思想就像土壤侵蚀一样，它侵蚀前景物体的边界(尽量使前景保持白色)。它是做什么的呢?
 内核滑动通过图像(在2D卷积中)。原始图像中的一个像素(无论是1还是0)只有当内核下的所有 像素都是1时才被认为是1，否则它就会被侵蚀(变成0)。
 结果是，根据内核的大小，边界附近的所有像素都会被丢弃。因此，前景物体的厚度或大小减 小，或只是图像中的白色区域减小。它有助于去除小的白色噪声(正如我们在颜色空间章节中看到 的)，分离两个连接的对象等。
 */
void Erosion(int , void*){
    int erosion_type = 0;
    if(erosion_elem == 0){
        erosion_type = MORPH_RECT;
    }else if(erosion_elem == 1){
        erosion_type = MORPH_CROSS;
    }else if(erosion_elem == 2){
        erosion_type = MORPH_ELLIPSE;
    }
    
    Mat element = getStructuringElement(erosion_type, Size(2 * erosion_size + 1 , 2 * erosion_size + 1) , Point(erosion_size , erosion_size));
    
    erode(src, erosion_dst, element);
    imshow("Erosion Demo", erosion_dst);
    
}

/*
 扩张
 
 它与侵蚀正好相反。如果内核下的至少一个像素为“ 1”，则像素元素为“ 1”。因此，它会增加图像中的白色区域或增加前景对象的大小。
 通常，在消除噪音的情况下，腐蚀后会膨胀。因为腐蚀会消除白噪声，但也会缩小物体。
 因此，我们对其进行了扩展。由于噪音消失了，它们不会回来， 但是我们的目标区域增加了。在连接对象的损坏部分时也很有用。
 */
void Dilation(int , void*){
    int dilation_type = 0;
    if(dilation_elem == 0){
        // 矩形
        dilation_type = MORPH_RECT;
    }else if(dilation_elem == 1){
        // 交叉型
        dilation_type = MORPH_CROSS;
    }else if(dilation_elem == 2){
        // 椭圆形
        dilation_type = MORPH_ELLIPSE;
    }
    
    // 返回指定形状和尺寸的结构元素
    Mat element = getStructuringElement(dilation_type, Size(2 * dilation_size + 1 , 2 * dilation_size + 1) , Point(dilation_size , dilation_size));
    
    dilate(src, dilation_dst, element);
    imshow("Dilation Demo", dilation_dst);
}
