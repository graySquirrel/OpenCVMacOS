//
//  main.cpp
//  OpenCVTest
//
//  Created by Fritz Ebner on 4/3/19.
//  Copyright © 2019 Fritz Ebner. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

double distance(int x1, int y1, int x2, int y2) {
    return pow(pow(x2-x1, 2) + pow(y2-y1, 2), 0.5);
}

double slope(int x1, int y1, int x2, int y2) {
    if (x2==x1) {
        return ((double)(100000));
    }
    return (float((y2-y1))/(x2-x1));
}

tuple<double, double, double, double, double> slopeInterceptLength(int x1, int y1, int x2, int y2) {
    double b, d, m, yIntrAtW;
                tuple <double, double, double, double, double> stuff;
    m = slope(x1, y1, x2, y2);
    b = (y1-(m*x1));
    yIntrAtW = ((m*360)+b);
    d = distance(x1, y1, x2, y2);
    stuff = make_tuple(m,b,d,abs(m),yIntrAtW);
    return (stuff);
}

int main(int argc, const char * argv[]) {
        
        // Create a VideoCapture object and open the input file
        // If the input is the web camera, pass 0 instead of the video file name
        VideoCapture cap("/Users/fritzebner/Documents/OpenCVTest/OpenCVTest/IMG_0181.mp4");
        
        // Check if camera opened successfully
        if(!cap.isOpened()){
            cout << "Error opening video stream or file" << endl;
            return -1;
        }
        
        while(1){
            
            Mat frame;
            // Capture frame-by-frame
            cap >> frame;
            
            // If the frame is empty, break immediately
            if (frame.empty())
                break;
            // #1 get shape of input image, then reduce by 2x for faster processing
            int width = frame.cols;
            int height = frame.rows;
            int channels = frame.channels();
            width = int(width/2);
            height = int(height/2);
            Size newSize(width,height);
            resize(frame, frame, newSize);
            // #2 convert to grayscale for processing
            Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            // #3 run lineSegmentDetector on whole image
            Ptr<LineSegmentDetector> lsd = createLineSegmentDetector();
            vector<Vec4f> lines;
            lsd->detect(gray, lines);
            lsd->drawSegments(frame, lines);
            //cout << lines.size() << '\n';
            if (lines.size() > 0) {
                // #4 filter line segments to specific set of slopes and Y intercepts
                vector<Vec4f> linesF = filterLines(lines, 2);
                
            }
            // Display the resulting frame
            imshow( "Frame", frame );
            
            // Press  ESC on keyboard to exit
            char c=(char)waitKey(25);
            if(c==27)
                break;
        }
        
        // When everything done, release the video capture object
        cap.release();
        
        // Closes all the frames
        destroyAllWindows();
        
        return 0;
}
