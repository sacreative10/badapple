#include <iostream>
#include <vector>
#include <opencv4/opencv2/opencv.hpp>
#include <boost/thread/thread.hpp>
#include <ncursesw/curses.h>

void outputFrame(const std::vector<std::vector<char>>& frame){
    // use ncurses to output the frame
    for (int i = 0; i < frame.size(); i++){
        for (int j = 0; j < frame[i].size(); j++){
            mvaddch(i, j, frame[i][j]);
        }
    }
    refresh();
}

std::vector<std::vector<char>> testFrame(int width, int height){
    auto frame = std::vector<std::vector<char>>(height, std::vector<char>(width));
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (i == 0 || i == height - 1){
                frame[i][j] = '-';
            } else if (j == 0 || j == width - 1){
                frame[i][j] = '|';
            } else {
                frame[i][j] = ' ';
            }
        }
    }
    return frame;
}

std::vector<std::vector<char>> getFrame(cv::VideoCapture& cap, int width = 600){
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()){
        std::cerr << "Error reading frame" << std::endl;
        return {};
    }
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    cv::resize(frame, frame, cv::Size(width, width * 1080 / 1920));
    // show the frame
    cv::imshow("Frame", frame);
    cv::waitKey(1);
    auto frameData = std::vector<std::vector<char>>(frame.rows, std::vector<char>(frame.cols));
    for (int i = 0; i < frame.rows; i++){
        for (int j = 0; j < frame.cols; j++){
            frameData[i][j] = frame.at<uchar>(i, j) > 128 ? ' ' : '#';
        }
    }
    return frameData;
}

int main()
{
    // get video dimensions
    char* filename = "../bad_apple.mp4";
    std::cout.sync_with_stdio(false);
    cv::VideoCapture cap(filename);
    if (!cap.isOpened()){
        std::cerr << "Error opening video stream or file" << std::endl;
        return -1;
    }
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "width: " << width << ", height: " << height << std::endl;

    // show the video in the dimensions of the frame
    cv::namedWindow("Frame", cv::WINDOW_NORMAL);
    // init ncurses
    initscr();
    for (;;)
    {
        auto frame = getFrame(cap, 200);
        if (frame.empty()){
            break;
        }
        outputFrame(frame);
        // sleep for 1/60th of a second
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000/60));
    }
    endwin();
    cap.release();

    return 0;
}
