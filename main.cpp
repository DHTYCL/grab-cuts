#include <opencv2/opencv.hpp>
#include"highgui.h"
#include"cv.h"
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include<vector>
#include<stack>
using namespace cv;
using namespace std;

#include"highgui.h"
#include"cv.h"
using namespace cv;

Rect selection;
Mat img,img0;
Point prePt(-1,-1);
void onMouse(int event,int x,int y,int flags,void* param){
    if (event==CV_EVENT_LBUTTONDOWN){
        prePt = Point(x, y);
    }
    else if (event == CV_EVENT_MOUSEMOVE && (flags && CV_EVENT_FLAG_LBUTTON)){
        img = img0.clone();
        rectangle(img, Rect(prePt.x, prePt.y, abs(x - prePt.x), abs(y - prePt.y)), Scalar(0,0,255),3);
        selection = Rect(prePt.x, prePt.y, abs(x - prePt.x), abs(y - prePt.y));

    }
    else if (event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON)){

        prePt=Point(-1, -1);
    }
    imshow("source", img);
}

int main(){


    VideoCapture capture(0);

    //img = imread("/home/aitian/4.jpg",1);
    capture>>img;  //读取当前帧
    img0 = img.clone();
    namedWindow("source",1);
    imshow("source", img);
    setMouseCallback("source", onMouse);
    Mat result; // 4种可能结果
    Mat bgModel, fgModel; // 背景/前景
    for (;;){
        int c = waitKey(10);
        if ((char)c == 'p'){
            grabCut(img0, result, selection, bgModel, fgModel, 3, GC_INIT_WITH_RECT);
            compare(result,GC_PR_FGD,result,CMP_EQ);//得到前景mask
            Mat foreground(img.size(),CV_8UC3,Scalar::all(255));
            img0.copyTo(foreground,result);
            imshow("grabcut",foreground);
        }
        if (char(c) == 'q') return 0;


    }
    return 0;
}












//vector<Point>connects={Point(0,-1),Point(1,0),Point(0,1),
//                       Point(-1,0)
//                      };
////计算两个点间的欧式距离
//double get_dist(Mat frame,Point seed_location1,Point seed_location2)
//{
//    int blue1=frame.at<Vec3b>(seed_location1.x,seed_location1.y)[0];
//    int green1=frame.at<Vec3b>(seed_location1.x,seed_location1.y)[1];
//    int red1=frame.at<Vec3b>(seed_location1.x,seed_location1.y)[2];

//    int blue2=frame.at<Vec3b>(seed_location2.x,seed_location2.y)[0];
//    int green2=frame.at<Vec3b>(seed_location2.x,seed_location2.y)[1];
//    int red2=frame.at<Vec3b>(seed_location2.x,seed_location2.y)[2];
//    double res=  pow(blue1-blue2,2)+pow(green1-green2,2)+pow(red1-red2,2);
//    return sqrt(res);
//}
//void on_MouseHandle(int event,int x,int y,int flags,void*param)
//{
//    Mat&frame=*(Mat*)param;
//    if(event==EVENT_LBUTTONDOWN)
//    {
//        Point start_point(y,x);
//        int row=frame.rows;
//        int col=frame.cols;
//        cout<<row<<" "<<col<<endl;//439  d648
//        //标记，判断种子是否已经生长
//        vector<vector<bool>>img_mark(row,vector<bool>(col,false));
//        Mat img_re;
//        frame.copyTo(img_re);
//        for(int i=0;i<row;i++)
//        {
//            for(int j=0;j<col;j++)
//            {
//               img_re.at<Vec3b>(i,j)[0]=0;
//               img_re.at<Vec3b>(i,j)[1]=0;
//               img_re.at<Vec3b>(i,j)[2]=0;
//            }
//        }
//        //imshow("vedio",img_re);  //显示当前帧
//        //随即取一点作为种子点
//        stack<Point>seed_list;
//        seed_list.push(start_point);
//        double T = 6;//阈值
//        bool class_k = true;//类别
//        //生长一个类
//        while(!seed_list.empty())
//        {
//            Point seed_tmp = seed_list.top();
//            seed_list.pop();
//            img_mark[seed_tmp.x][seed_tmp.y]=class_k;
//            //# 遍历8邻域
//            for(int j=0;j<4;j++)
//            {
//             int tmpX=seed_tmp.x+connects[j].x;
//             int tmpY=seed_tmp.y+connects[j].y;
//             if (tmpX < 0 ||tmpY < 0 ||tmpX >= row|| tmpY >= col)
//                continue;
//              double dist = get_dist(frame,seed_tmp, Point(tmpX, tmpY));

//           //在种子集合中满足条件的点进行生长
//            if (dist < T && img_mark[tmpX][tmpY] == false)
//            {
//                 img_re.at<Vec3b>(tmpX,tmpY)[0] = frame.at<Vec3b>(tmpX, tmpY)[0];
//                 img_re.at<Vec3b>(tmpX,tmpY)[1] = frame.at<Vec3b>(tmpX, tmpY)[1];
//                 img_re.at<Vec3b>(tmpX,tmpY)[2] = frame.at<Vec3b>(tmpX, tmpY)[2];
//                 img_mark[tmpX][tmpY] = true;
//                 seed_list.push(Point(tmpX, tmpY));

//            }
//            }
//        }
//        imshow("vedio",img_re);  //显示当前帧
//    }
//}

//int main( )
//{
//            VideoCapture capture(0);

//            //【2】循环显示每一帧
//            while(1)
//            {
//                Mat frame=imread("/home/aitian/1.jpg");//定义一个Mat变量，用于存储每一帧的图像
//                capture>>frame;  //读取当前帧
//                imshow("vedio2a",frame);
//                //若视频播放完成，退出循环
//                if (frame.empty())
//                {
//                    return 0;
//                }
//                setMouseCallback("vedio2a",on_MouseHandle,&frame);




//               // imshow("vedio",frame);  //显示当前帧


//                //blur( frame, frame, Size(3,3) );
//                //cvtColor( frame, frame, COLOR_BGR2GRAY );
//                //Canny( frame, frame, 3, 90,3 );
//                //imshow("vedio",frame);  //显示当前帧
//                waitKey(300);  //延时30ms
//           }
//    waitKey(0);

//    return 0;
//}
