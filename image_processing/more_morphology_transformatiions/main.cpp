#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace cv;
Mat src, dst;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

const char* window_name = "Morphology Transformations Demo";

void Morphology_Operations(int , void*);

/*
 [OpenCV学习笔记-顶帽、黑帽、形态学梯度](https://blog.csdn.net/qq_36387683/article/details/80489631)
 
 开运算---先腐蚀，后膨胀。去除图像中小的亮点（CV_MOP_OPEN）；

 闭运算---先膨胀，后腐蚀。去除图像中小的暗点（CV_MOP_CLOSE）；

 形态学梯度---原图膨胀图像 — 原图腐蚀图像（CV_MOP_GRADIENT）；

 顶帽---原图像 — 原图像开操作。保留小亮点，去除大亮点。（CV_MOP_TOPHAT）；

 黑帽---原图像闭操作 — 原图像（CV_MOP_BLACKHAT）；
 
 [图像处理中常见的形态学方法](https://zhuanlan.zhihu.com/p/110787009)

 */
int main(int argc, const char * argv[]) {
    CommandLineParser parser(argc , argv , "{@input | /Users/wecut1260/Desktop/hello.png}");
    src = imread(samples::findFile(parser.get<String>("@input")) , IMREAD_COLOR);
    
    if(src.empty())
    {
        std::cout << "error image" << std::endl;
        return -1;
    }

    
    namedWindow(window_name , WINDOW_AUTOSIZE);
    
    createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator, Morphology_Operations );
    createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name, &morph_elem , max_elem, Morphology_Operations );
    createTrackbar( "Kernel size:\n 2n +1", window_name,
                    &morph_size, max_kernel_size,
                    Morphology_Operations );
    
    Morphology_Operations(0, 0);
    waitKey(0);
    return 0;
  
}

void Morphology_Operations(int , void*){
    int operation = morph_operator + 2;
    Mat element = getStructuringElement(morph_elem, Size(2 * morph_size + 1 , 2 * morph_size + 1) , Point(morph_size , morph_size));
    
    morphologyEx(src, dst, operation, element);
    imshow(window_name, dst);
}
