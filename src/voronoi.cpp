#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout << "\nThis program demostrates iterative construction of\n"
    "delaunay triangulation and voronoi tesselation.\n"
    "It draws a random set of points in an image and then delaunay triangulates them.\n"
    "Usage: \n"
    "./delaunay \n"
    "\nThis program builds the traingulation interactively, you may stop this process by\n"
    "hitting any key.\n";
}

static void draw_subdiv_point(Mat& img, Point2f fp, Scalar color)
{
    //circle(img, fp, 1, color, FILLED, LINE_8, 0);
    circle(img, fp, 1, color, 1, 8, 0);
}

static void draw_subdiv(Mat& img, Subdiv2D& subdiv, Scalar delaunay_color)
{
#if 1
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);
    
    for (size_t i = 0; i < triangleList.size(); i++)
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        //line(img, pt[0], pt[1], delaunay_color, 1, LINE_AA, 0);
        //line(img, pt[1], pt[2], delaunay_color, 1, LINE_AA, 0);
        //line(img, pt[2], pt[0], delaunay_color, 1, LINE_AA, 0);
        int x1 = (pt[0].x + pt[1].x) / 2;
        int y1 = (pt[0].y + pt[1].y) / 2;
        int x2 = (pt[0].x + pt[2].x) / 2;
        int y2 = (pt[0].y + pt[2].y) / 2;
        int x3 = (pt[1].x + pt[2].x) / 2;
        int y3 = (pt[1].y + pt[2].y) / 2;
        Point mid1(x1, y1);
        Point mid2(x2, y2);
        Point mid3(x3, y3);
//        Point mid1 = (pt[0] + pt[1]) / 2;
//        Point mid2 = (pt[0] + pt[2]) / 2;
//        Point mid3 = (pt[1] + pt[2]) / 2;
        draw_subdiv_point(img, mid1, Scalar(0, 0, 0));
        draw_subdiv_point(img, mid2, Scalar(0, 0, 0));
        draw_subdiv_point(img, mid3, Scalar(0, 0, 0));
    }
#else
    vector<Vec4f> edgeList;
    subdiv.getEdgeList(edgeList);
    for (size_t i = 0; i < edgeList.size(); i++)
    {
        Vec4f e = edgeList[i];
        Point pt0 = Point(cvRound(e[0]), cvRound(e[1]));
        Point pt1 = Point(cvRound(e[2]), cvRound(e[3]));
        //line(img, pt0, pt1, delaunay_color, 1, LINE_AA, 0);
        draw_subdiv_point(img, (pt0 + pt1) / 2, Scalar(255, 255, 255));
    }
#endif
}

static void locate_point(Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar active_color)
{
    int e0 = 0, vertex = 0;
    
    subdiv.locate(fp, e0, vertex);
    
    if (e0 > 0)
    {
        int e = e0;
        do
        {
            Point2f org, dst;
            if (subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst) > 0)
                line(img, org, dst, active_color, 3, 1, 0);
            
            e = subdiv.getEdge(e, Subdiv2D::NEXT_AROUND_LEFT);
        } while (e != e0);
    }
    
    //draw_subdiv_point(img, fp, active_color);
}


static void paint_voronoi(Mat& img, Subdiv2D& subdiv)
{
    vector<vector<Point2f> > facets;
    vector<Point2f> centers;
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);
    
    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);
    
    for (size_t i = 0; i < facets.size(); i++)
    {
        ifacet.resize(facets[i].size());
        for (size_t j = 0; j < facets[i].size(); j++)
            ifacet[j] = facets[i][j];
        
        Scalar color;
        color[0] = rand() & 255;
        color[1] = rand() & 255;
        color[2] = rand() & 255;
        fillConvexPoly(img, ifacet, color, 8, 0);
        
        ifacets[0] = ifacet;
        polylines(img, ifacets, true, Scalar(), 1, 8, 0);
        circle(img, centers[i], 3, Scalar(), 1, 8, 0);
    }
}


int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, "{help h||}");
//    if (parser.has("help"))
//    {
//        help();
//        return 0;
//    }
    
    Scalar active_facet_color(0, 0, 255), delaunay_color(255, 255, 255);
    //Rect rect(0, 0, 600, 600);
    
    //Subdiv2D subdiv(rect);
    //Mat img(rect.size(), CV_8UC3);
    
    //img = Scalar::all(0);
    Mat img2 = imread("Contoured Map.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    
    vector<Point2f> pointSet;
    for (int i = 0; i < img2.rows; i++)
    {
        for (int j = 0; j < img2.cols; j++)
        {
            if (img2.at<unsigned char>(i, j) == 0 && (i > 100 && i < img2.rows - 100) && (j > 100 && j < img2.cols - 100))
            {
                Point2f fp(j, i);
                pointSet.push_back(fp);
            }
        }
    }
    //cout << img2;
    //cout << img2.cols << " " << img2.rows << endl;
    //string win = "Delaunay Demo";
    //imshow(win, img);
    
    /*for (int i = 0; i < 200; i++)
     {
     Point2f fp((float)(rand() % (rect.width - 10) + 5),
     (float)(rand() % (rect.height - 10) + 5));
     
     locate_point(img, subdiv, fp, active_facet_color);
     imshow(win, img);
     
     if (waitKey(100) >= 0)
     break;
     
     subdiv.insert(fp);
     
     img = Scalar::all(0);
     draw_subdiv(img, subdiv, delaunay_color);
     imshow(win, img);
     
     if (waitKey(100) >= 0)
     break;
     }*/
    
    Rect rect(0, 0, img2.cols, img2.rows);
    Subdiv2D subdiv(rect);
    Mat img3(rect.size(), CV_8UC3);
    string win = "Delaunay Demo";
    img3 = Scalar::all(255);
    imshow(win, img3);
    
    for (int i = 0; i < pointSet.size(); i++)
    {
        Point2f fp = pointSet[i];
        locate_point(img3, subdiv, fp, active_facet_color);
        //imshow(win, img3);
        
        //if (waitKey(1) >= 0)
        //break;
        
        subdiv.insert(fp);
        
        img3 = Scalar::all(255);
        draw_subdiv(img3, subdiv, delaunay_color);
        imshow(win, img3);
        
        if (waitKey(1) >= 0)
            break;
    }
    imshow(win, img3);
    /*img = Scalar::all(0);
     paint_voronoi(img, subdiv);
     string win2 = "Voronoi Demo";
     imshow(win2, img);*/
    
    waitKey(0);
    
    return 0;
}
