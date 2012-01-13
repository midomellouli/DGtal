/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file visuDistanceTransform.cpp
 * @ingroup Examples
 * @author Bertrand Kerautret (\c kerautre@loria.fr )
 * LORIA (CNRS, UMR 7503), University of Nancy, France
 *
 * @date 2011/01/04
 *
 * An example file named qglViewer.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include <QtGui/qapplication.h>

#include "DGtal/base/Common.h"
#include "DGtal/kernel/SpaceND.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/io/readers/VolReader.h"
#include "DGtal/io/viewers/Viewer3D.h"

#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/GradientColorMap.h"
#include "DGtal/io/DrawWithDisplay3DModifier.h"

#include "DGtal/kernel/SpaceND.h"
#include "DGtal/kernel/domains/HyperRectDomain.h"
#include "DGtal/images/ImageSelector.h"

#include "DGtal/geometry/nd/volumetric/SeparableMetricTraits.h"
#include "DGtal/geometry/nd/volumetric/DistanceTransformation.h"


#include "ConfigExamples.h"


///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////


/** 
 * Set to a given value a random set of @param nb points.
 * 
 * @param image the  image
 * @param nb the number of random points to insert
 * @param value the value to add at each random point
 */
template<typename Image>
void randomSeeds(Image &image, const unsigned int nb, const int value)
{
  typename Image::Point p, low = image.lowerBound();
  typename Image::Vector ext;
  srand ( time(NULL) );

  ext = image.extent();

  for (unsigned int k = 0 ; k < nb; k++)
  {
    for (unsigned int dim = 0; dim < Image::dimension; dim++)
      p[dim] = rand() % (ext[dim]) +  low[dim];
    
    image.setValue(p, value);
  }
}

int main( int argc, char** argv )
{
  
  std::string inputFilename = examplesPath + "samples/Al.100.vol";
  
 //------------

 typedef SpaceND<3> Space4Type;
 typedef HyperRectDomain<Space4Type> TDomain;

 typedef TDomain::Point Point;

  
 QApplication application(argc,argv);
 Viewer3D viewer;
 viewer.setWindowTitle("simpleViewer");
 viewer.show();


 
 //Default image selector = STLVector
 typedef ImageSelector<TDomain, unsigned char>::Type Image;
 Image image = VolReader<Image>::importVol( inputFilename );
 TDomain domain(image.lowerBound(), image.upperBound());


 Image imageSeeds (image.lowerBound(), image.upperBound());
 for ( Image::Iterator it = imageSeeds.begin(), itend = imageSeeds.end();it != itend; ++it)
   (*it)=1;
 Z3i::Point p0(10,10,10);
 //imageSeeds.setValue(p0, 0 );
 randomSeeds(imageSeeds, 70, 0);


 //Distance transformation computation
 typedef ImageSelector<TDomain, long int>::Type ImageLong;

 typedef  DistanceTransformation<Image, 2> DTL2;
 typedef  DistanceTransformation<Image, 0> DTLInf;
 typedef  DistanceTransformation<Image, 1> DTL1;
 
 DTL2 dtL2;
 DTLInf dtLinf;
 DTL1 dtL1;

 
 dtL1.checkTypesValidity ( imageSeeds );
 DTL1::OutputImage resultL1 = dtL1.compute ( imageSeeds );
  


 
 unsigned int min = 0;
 unsigned int max = 0;
 for(DTL1::OutputImage::ConstIterator it = resultL1.begin(), itend=resultL1.end();
     it!=itend;
     ++it)
   {
     if(  (*it) < min )   
       min=(*it);
     if( (*it) > max ) 
       max=(*it);
   }
     
     
  GradientColorMap<long> gradient( 0,30);
  gradient.addColor(Color::Red);
  gradient.addColor(Color::Yellow);
  gradient.addColor(Color::Green);
  gradient.addColor(Color::Cyan);
  gradient.addColor(Color::Blue);
  gradient.addColor(Color::Magenta);
  gradient.addColor(Color::Red);  
 

  viewer << SetMode3D( (*(domain.begin())).className(), "Paving" );
  
  for(TDomain::ConstIterator it = domain.begin(), itend=domain.end();
     it!=itend;
     ++it){
   
   unsigned int valDist= resultL1( (*it) );     
   Color c= gradient(valDist);
   
   if(resultL1(*it)<=30 ){
     viewer << CustomColors3D(Color((float)(c.red()), 
            (float)(c.green()),
            (float)(c.blue(),205)), 
            Color((float)(c.red()), 
            (float)(c.green()),
            (float)(c.blue()),205));
     viewer << *it ;
   }     
 }
 
  //viewer << ClippingPlane(1,0,0,-60);
 viewer<< Viewer3D::updateDisplay;
 
 return application.exec();
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
