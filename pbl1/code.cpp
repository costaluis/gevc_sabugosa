#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char ** argv)
{
    //Captura imagem do vídeo
    VideoCapture cap("painel1.mp4");

    if(!cap.isOpened())
    {
        cout << "O vídeo não pode ser aberto ..." << endl;
        return -1;
    }

    int iLowH = 0;
    int iHighH = 22;

    int iLowS = 5;
    int iHighS = 255;
    
    int iLowV = 233;
    int iHighV = 255;

    int iLowH_2 = 0;
    int iHighH_2 = 255;

    int iLowS_2 = 1;
    int iHighS_2 = 108;
    
    int iLowV_2 = 218;
    int iHighV_2 = 255;

    namedWindow("PAINEL",WINDOW_NORMAL);
    namedWindow("PAINEL THRESHOLDED");

    Mat ImgPainel;

    ImgPainel = imread("painel.png",IMREAD_COLOR);

    blur(ImgPainel,ImgPainel,Size(3,3),Point(-1,-1),BORDER_DEFAULT);

    Mat PainelHSV;
    Mat PainelThresholded;

    cvtColor(ImgPainel,PainelHSV,COLOR_BGR2HSV);

    inRange(PainelHSV, Scalar(iLowH_2, iLowS_2, iLowV_2), Scalar(iHighH_2, iHighS_2, iHighV_2), PainelThresholded);

    erode(PainelThresholded, PainelThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( PainelThresholded, PainelThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    imshow("PAINEL",ImgPainel);
    imshow("PAINEL THRESHOLDED", PainelThresholded);
    waitKey();
    destroyAllWindows();

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal);
        if(!bSuccess)
        {
            cout << "Impossivel ler o video" << endl;
            break;
        }

        blur(imgOriginal,imgOriginal,Size(2,2),Point(-1,-1),BORDER_DEFAULT);


        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
        namedWindow("Thresholded Image",WINDOW_NORMAL);
        namedWindow("Original",WINDOW_NORMAL);
        imshow("Thresholded Image", imgThresholded); //show the thresholded image

        //imgOriginal = imgOriginal + imgLines;
        imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //espera pela tecla 'esc' ser pressionada, se for o loop é quebrado
        {
            cout << "tecla ESC foi pressionado pelo usuario" << endl;
            break; 
        }
    
    }
       
}