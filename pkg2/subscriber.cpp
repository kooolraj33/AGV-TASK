#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat img1 = imread("ps1.jpg",CV_LOAD_IMAGE_COLOR);
int sum1_x=0,sum1_y=0,cnt1=0,xtargetcm,ytargetcm; 

void shade(float x1, float y1)
  {
  img1.at<Vec3b>(x1,y1).val[0]=255;
  img1.at<Vec3b>(x1,y1).val[1]=0;
  img1.at<Vec3b>(x1,y1).val[2]=0;
  imshow("OUTPUT",img1);
 }

void chatterCallback(const geometry_msgs::Point::ConstPtr& msg)
 {
  int i=0;
  int x=0,y=0;
  geometry_msgs:: Point p;

  p.x = msg->x;  // subscribed point input 
  p.y = msg->y;
  shade(p.x,p.y); //  signifies the marked points
  imshow("OUTPUT",img1);
  if(p.x==(xtargetcm+1) && p.y==(ytargetcm+1))
   {// end of path
    cout<<"yep!";
    waitKey(0);
  }
}

int main(int argc, char **argv)
{   
  cout<<"Subscribing...Please Wait\n "; 
   // time to subscribe
  namedWindow("OUTPUT", WINDOW_NORMAL);
  int i=0,j=0; 
    for(i=0;i<img1.rows;i++){
      for(j=0;j<img1.cols;j++){
        if(img1.at<Vec3b>(i,j)[2] ==255  && img1.at<Vec3b>(i,j)[0]==0&& img1.at<Vec3b>(i,j)[1]==0)
          {
	  sum1_x+=i; // centre of target green spot
	  sum1_y+=j;
          cnt1++;
        }
      }
    }
     xtargetcm = sum1_x/cnt1;
     ytargetcm = sum1_y/cnt1;
    ros::init(argc, argv, "subscriber");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("speaker", 1000, chatterCallback);
    ros::spin();
    return 0;
}
