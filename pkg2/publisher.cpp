#include "ros/ros.h"
#include "geometry_msgs/Point.h"
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

struct graphs{
		int x;
		int y;
	       }graph[40000];

Mat img1 = imread("ps1.jpg",CV_LOAD_IMAGE_COLOR);
int arr[200][200];// to store obstacles pixels as 1 and path as 0
long  int st_x,st_y; 
int isvisited[200][200];
int dummy=0,m=0; 
  int end_x,end_y;
 
void dfs(int at_x,int at_y)
{
  isvisited[at_x][at_y]=1;// marking as visited
  int i=0,j=0,flag=0;
  for(i=1;i>-2;i--)
     {	
    for(j=1;j>-2;j--)
       {
      if(at_x+i>=0 && at_y+j>=0 && isvisited[at_x+i][at_y+j]==0 && arr[at_x+i][at_y+j]!=0 && dummy==0 && at_x+i<=end_x && at_y+j<=end_y)
       {
        flag++;
	dfs(at_x+i,at_y+j);
	if(dummy==1)
	  break;
       }
      }
    if(dummy==1)
      break;
    }
   if(at_x==end_x && at_y==end_y){
    dummy=1;
    return;
  }
 // if(flag==0)
   //dfs(st_x,st_y);
  if(dummy==1)
    {
    graph[m].x=at_x + i;
    graph[m].y=at_y + j;
    m++;                 
    }                        
}

int main(int argc, char **argv)
{
  int sum1_x=0,sum1_y=0,sum2_x=0,sum2_y=0;
  long  int st_x,st_y; 
  int count1=0,count2=0;
  int i=0,j=0,temp=0;
  Mat img = imread("ps1.jpg",CV_LOAD_IMAGE_COLOR); 
  for(i=0;i<200;i++)
    for(j=0;j<200;j++)
      isvisited[i][j]=0;   // setting all unvisited 

  for(i=0;i<200;i++)
     {
    for(j=0;j<200;j++)
        {
      if(img.at<Vec3b>(i,j)[2] == 237 && img.at<Vec3b>(i,j)[0] == 36&&img.at<Vec3b>(i,j)[1] == 28)
       {
	sum1_x+=i; // calculating centre of starting spot
	sum1_y+=j;
        count1++;
      }
      if(img.at<Vec3b>(i,j)[1] == 255 && img.at<Vec3b>(i,j)[2] ==0&& img.at<Vec3b>(i,j)[0] ==0)
      {
        sum2_x+=i;   // calculating centre of end spot
	sum2_y+=j;
        count2++;
      }
      if(img.at<Vec3b>(i,j)[0]==255 && img.at<Vec3b>(i,j)[1]==255 
	 && img.at<Vec3b>(i,j)[2]==255)
       {
         arr[i][j]=0;
      }
      else
       {
	arr[i][j]=1;
      }
    }
  }

  st_x = sum1_x/count1;
  st_y = sum1_y/count1;
  end_x = sum2_x/count2;
  end_y = sum2_y/count2;
  
  arr[st_x][st_y]=1;
  arr[end_x][end_y]=1;
  dfs(st_x,st_y);
 
  ros::init(argc, argv, "publisher"); 
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Point>("speaker", 1000);
  ros::Rate loop_rate(10);

  while (temp<m)
  {
    geometry_msgs:: Point msg;
    msg.x = graph[temp].x;
    msg.y = graph[temp].y ;
    cout<<"...Publishing "<<temp<<" path point"<<endl;
    chatter_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
    temp++;
  }
  return 0;
}
