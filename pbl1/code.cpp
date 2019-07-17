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

    //Variaveis para o vídeo

    int iLowH = 0;
    int iHighH = 22;

    int iLowS = 5;
    int iHighS = 255;
    
    int iLowV = 233;
    int iHighV = 255;

    //Variáveis para a imagem

    int iLowH_2 = 0;
    int iHighH_2 = 255;

    int iLowS_2 = 1;
    int iHighS_2 = 108;
    
    int iLowV_2 = 218;
    int iHighV_2 = 255;

    //Criação das janelas

    namedWindow("PAINEL",WINDOW_NORMAL);
    namedWindow("PAINEL THRESHOLDED",WINDOW_NORMAL);
    namedWindow("PAINEL HSV",WINDOW_NORMAL);

    Mat ImgPainel;

    //Leitura da imagem

    ImgPainel = imread("painel.png",IMREAD_COLOR);

    //Aplicação do efeito blur

    blur(ImgPainel,ImgPainel,Size(3,3),Point(-1,-1),BORDER_DEFAULT);

    Mat PainelHSV;
    Mat PainelThresholded;
 
    //Conversão de RGB para HSV

    cvtColor(ImgPainel,PainelHSV,COLOR_BGR2HSV);

    //Conversão de HSV para imagem Binária

    inRange(PainelHSV, Scalar(iLowH_2, iLowS_2, iLowV_2), Scalar(iHighH_2, iHighS_2, iHighV_2), PainelThresholded);

    //Aplicação de filtros

    erode(PainelThresholded, PainelThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( PainelThresholded, PainelThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //Joga imagem nas janelas

    Mat src_gray;

  /// Convert it to gray
  cvtColor( ImgPainel, src_gray, CV_BGR2GRAY );

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 50, 40, 0, 0 );

  cout << "Numero de circulos:" << circles.size() << endl;

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( ImgPainel, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( ImgPainel, center, radius, Scalar(0,0,255), 3, 8, 0 );
   }

  /// Show your results
  namedWindow( "Hough Circle Transform Demo", WINDOW_NORMAL );
  imshow( "Hough Circle Transform Demo", src_gray );

    imshow("PAINEL",ImgPainel);
    imshow("PAINEL HSV",PainelHSV);
    imshow("PAINEL THRESHOLDED", PainelThresholded);
    waitKey();
    destroyAllWindows();

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal);

        //Verifica se imagem pôde ser aberta
        if(!bSuccess)
        {
            cout << "Impossivel ler o video" << endl;
            break;
        }

        blur(imgOriginal,imgOriginal,Size(3,3),Point(-1,-1),BORDER_DEFAULT);


        Mat imgHSV;
        Mat imgGray;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        cvtColor(imgOriginal, imgGray, COLOR_BGR2GRAY);

        HoughCircles( imgGray, circles, CV_HOUGH_GRADIENT, 1, imgGray.rows/8, 50, 40, 0, 0 );

  cout << "Numero de circulos:" << circles.size() << endl;

    namedWindow("CIRCLES",WINDOW_NORMAL);

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( imgGray, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( imgGray, center, radius, Scalar(0,0,255), 3, 8, 0 );
   }
        imshow("CIRCLES",imgGray);
    
        namedWindow("Thresholded Image",WINDOW_NORMAL);
        namedWindow("HSV",WINDOW_NORMAL);
        namedWindow("Original",WINDOW_NORMAL);
        imshow("Thresholded Image", imgThresholded); //show the thresholded image

        //imgOriginal = imgOriginal + imgLines;
        imshow("Original", imgOriginal); //show the original image
        imshow("HSV",imgHSV);

        if (waitKey(60) == 27) //espera pela tecla 'esc' ser pressionada, se for o loop é quebrado
        {
            cout << "tecla ESC foi pressionado pelo usuario" << endl;
            break; 
        }
    
    }
       
}