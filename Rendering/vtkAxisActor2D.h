/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkAxisActor2D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkAxisActor2D - Create an axis with tick marks and labels
// .SECTION Description
// vtkAxisActor2D creates an axis with tick marks, labels, and/or a title,
// depending on the particular instance variable settings. vtkAxisActor2D is
// a 2D actor; that is, it is drawn on the overlay plane and is not
// occluded by 3D geometry. To use this class, you typically specify two
// points defining the start and end points of the line (x-y definition using
// vtkCoordinate class), the number of labels, and the data range
// (min,max). You can also control what parts of the axis are visible
// including the line, the tick marks, the labels, and the title.  You can 
// also specify the label format (a printf style format).
//
// This class decides what font size to use and how to locate the labels. It
// also decides how to create reasonable tick marks and labels. The number
// of labels and the range of values may not match the number specified, but
// should be close.
//
// Labels are drawn on the "right" side of the axis. The "right" side is
// the side of the axis on the right as you move from Point1 to Point2. The
// way the labels and title line up with the axis and tick marks depends on
// whether the line is considered horizontal or vertical.
//
// The instance variables Point1 and Point2 are instances of vtkCoordinate.
// What this means is that you can specify the axis in a variety of coordinate
// systems. Also, the axis does not have to be either horizontal or vertical.
// The tick marks are created so that they are perpendicular to the axis.
//
// Set the text property/attributes of the title and the labels through the 
// vtkTextProperty objects associated to this actor.
//
// .SECTION See Also
// vtkAxisActor3D can be used to create axes in world coordinate space.
// vtkActor2D vtkTextMapper vtkPolyDataMapper2D vtkScalarBarActor
// vtkCoordinate vtkTextProperty

#ifndef __vtkAxisActor2D_h
#define __vtkAxisActor2D_h

#include "vtkActor2D.h"

class vtkPolyDataMapper2D;
class vtkPolyData;
class vtkTextMapper;
class vtkTextProperty;

#define VTK_MAX_LABELS 25

class VTK_RENDERING_EXPORT vtkAxisActor2D : public vtkActor2D
{
public:
  vtkTypeRevisionMacro(vtkAxisActor2D,vtkActor2D);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Instantiate object.
  static vtkAxisActor2D *New();
  
  // Description:
  // Specify the position of the first point defining the axis.
  vtkViewportCoordinateMacro(Point1);
  
  // Description:
  // Specify the position of the second point defining the axis. Note that
  // the order from Point1 to Point2 controls which side the tick marks
  // are drawn on (ticks are drawn on the right, if visible).
  vtkViewportCoordinateMacro(Point2);
  
  // Description:
  // Specify the (min,max) axis range. This will be used in the generation
  // of labels, if labels are visible.
  vtkSetVector2Macro(Range,float);
  vtkGetVectorMacro(Range,float,2);

  // Description:
  // Set/Get the number of annotation labels to show.
  vtkSetClampMacro(NumberOfLabels, int, 2, VTK_MAX_LABELS);
  vtkGetMacro(NumberOfLabels, int);
  
  // Description:
  // Set/Get the format with which to print the labels on the scalar
  // bar.
  vtkSetStringMacro(LabelFormat);
  vtkGetStringMacro(LabelFormat);

  // Description:
  // Set/Get the flag that controls whether the labels and ticks are
  // adjusted for "nice" numerical values to make it easier to read 
  // the labels. The adjustment is based in the Range instance variable.
  // Call GetAdjustedRange and GetAdjustedNumberOfLabels to get the adjusted
  // range and number of labels.
  vtkSetMacro(AdjustLabels, int);
  vtkGetMacro(AdjustLabels, int);
  vtkBooleanMacro(AdjustLabels, int);
  virtual float *GetAdjustedRange()
    { 
      this->UpdateAdjustedRange();
      return this->AdjustedRange; 
    }
  virtual void GetAdjustedRange(float &_arg1, float &_arg2)
    { 
      this->UpdateAdjustedRange();
      _arg1 = this->AdjustedRange[0];
      _arg2 = this->AdjustedRange[1];
    }; 
  virtual void GetAdjustedRange(float _arg[2]) 
    { 
      this->GetAdjustedRange(_arg[0], _arg[1]);
    } 
  virtual int GetAdjustedNumberOfLabels()
    {
      this->UpdateAdjustedRange();
      return this->AdjustedNumberOfLabels; 
    }

  // Description:
  // Set/Get the title of the scalar bar actor,
  vtkSetStringMacro(Title);
  vtkGetStringMacro(Title);

  // Description:
  // Set/Get the title text property.
  virtual void SetTitleTextProperty(vtkTextProperty *p);
  vtkGetObjectMacro(TitleTextProperty,vtkTextProperty);
  
  // Description:
  // Set/Get the labels text property.
  virtual void SetLabelTextProperty(vtkTextProperty *p);
  vtkGetObjectMacro(LabelTextProperty,vtkTextProperty);
      
  // Description:
  // Set/Get the font family. Three font types are allowed: Arial (VTK_ARIAL),
  // Courier (VTK_COURIER), and Times (VTK_TIMES).
  // Warning: these functions remain for backward compatibility. Use the
  // vtkTextProperty through the (Set/Get)(Title/Label)TextProperty() methods.
  virtual void SetFontFamily(int val);
  virtual int GetFontFamily();
  void SetFontFamilyToArial()   { this->SetFontFamily(VTK_ARIAL);  };
  void SetFontFamilyToCourier() { this->SetFontFamily(VTK_COURIER);};
  void SetFontFamilyToTimes()   { this->SetFontFamily(VTK_TIMES);  };

  // Description:
  // Enable/disable text bolding.
  // Warning: these functions remain for backward compatibility. Use the
  // vtkTextProperty through the (Set/Get)(Title/Label)TextProperty() methods.
  virtual void SetBold(int val);
  virtual int GetBold();
  vtkBooleanMacro(Bold, int);

  // Description:
  // Enable/disable text italic.
  // Warning: these functions remain for backward compatibility. Use the
  // vtkTextProperty through the (Set/Get)(Title/Label)TextProperty() methods.
  virtual void SetItalic(int val);
  virtual int GetItalic();
  vtkBooleanMacro(Italic, int);

  // Description:
  // Enable/disable text shadows.
  // Warning: these functions remain for backward compatibility. Use the
  // vtkTextProperty through the (Set/Get)(Title/Label)TextProperty() methods.
  virtual void SetShadow(int val);
  virtual int GetShadow();
  vtkBooleanMacro(Shadow, int);
  
  // Description:
  // Set/Get the length of the tick marks (expressed in pixels or display
  // coordinates). 
  vtkSetClampMacro(TickLength, int, 0, 100);
  vtkGetMacro(TickLength, int);
  
  // Description:
  // Set/Get the offset of the labels (expressed in pixels or display
  // coordinates). The offset is the distance of labels from tick marks
  // or other objects.
  vtkSetClampMacro(TickOffset, int, 0, 100);
  vtkGetMacro(TickOffset, int);
  
  // Description:
  // Set/Get visibility of the axis line.
  vtkSetMacro(AxisVisibility, int);
  vtkGetMacro(AxisVisibility, int);
  vtkBooleanMacro(AxisVisibility, int);

  // Description:
  // Set/Get visibility of the axis tick marks.
  vtkSetMacro(TickVisibility, int);
  vtkGetMacro(TickVisibility, int);
  vtkBooleanMacro(TickVisibility, int);

  // Description:
  // Set/Get visibility of the axis labels.
  vtkSetMacro(LabelVisibility, int);
  vtkGetMacro(LabelVisibility, int);
  vtkBooleanMacro(LabelVisibility, int);

  // Description:
  // Set/Get visibility of the axis title.
  vtkSetMacro(TitleVisibility, int);
  vtkGetMacro(TitleVisibility, int);
  vtkBooleanMacro(TitleVisibility, int);

  // Description:
  // Set/Get the factor that controls the overall size of the fonts used
  // to label and title the axes. This ivar used in conjunction with
  // the LabelFactor can be used to control font sizes.
  vtkSetClampMacro(FontFactor, float, 0.1, 2.0);
  vtkGetMacro(FontFactor, float);

  // Description:
  // Set/Get the factor that controls the relative size of the axis labels
  // to the axis title.
  vtkSetClampMacro(LabelFactor, float, 0.1, 2.0);
  vtkGetMacro(LabelFactor, float);

  // Description:
  // Draw the axis. 
  int RenderOverlay(vtkViewport* viewport);
  int RenderOpaqueGeometry(vtkViewport* viewport);
  int RenderTranslucentGeometry(vtkViewport *) {return 0;}

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  void ReleaseGraphicsResources(vtkWindow *);

  // Description:
  // This method computes the range of the axis given an input range. 
  // It also computes the number of tick marks given a suggested number.
  // (The number of tick marks includes end ticks as well.)
  // The number of tick marks computed (in conjunction with the output
  // range) will yield "nice" tick values. For example, if the input range
  // is (0.25,96.7) and the number of ticks requested is 10, the output range
  // will be (0,100) with the number of computed ticks to 11 to yield tick
  // values of (0,10,20,...,100).
  static void ComputeRange(float inRange[2], 
                           float outRange[2],
                           int inNumTicks, 
                           int &outNumTicks, 
                           float &interval);

  // Description:
  // General method to computes font size from a representative size on the 
  // viewport (given by size[2]). The method returns the font size (in points)
  // and the string height/width (in pixels). It also sets the font size of the
  // instance of vtkTextMapper provided. The factor is used when you're trying
  // to create text of different size-factor (it is usually = 1 but you can
  // adjust the font size by making factor larger or smaller).
  static int SetFontSize(vtkViewport *viewport, 
                         vtkTextMapper *textMapper, 
                         int *targetSize, 
                         float factor, 
                         int *stringSize);
  static int SetMultipleFontSize(vtkViewport *viewport, 
                                 vtkTextMapper **textMappers, 
                                 int nbOfMappers, 
                                 int *targetSize,
                                 float factor, 
                                 int *stringSize);

  // Description:
  // Shallow copy of an axis actor. Overloads the virtual vtkProp method.
  void ShallowCopy(vtkProp *prop);

protected:
  vtkAxisActor2D();
  ~vtkAxisActor2D();

  vtkCoordinate *Point1Coordinate;
  vtkCoordinate *Point2Coordinate;

  vtkTextProperty *TitleTextProperty;
  vtkTextProperty *LabelTextProperty;

  char  *Title;
  float Range[2];
  float AdjustedRange[2];
  int   NumberOfLabels;
  int   AdjustedNumberOfLabels;
  char  *LabelFormat;
  int   NumberOfLabelsBuilt;
  int   AdjustLabels;
  float FontFactor;
  float LabelFactor;
  int   TickLength;
  int   TickOffset;

  int   AxisVisibility;
  int   TickVisibility;
  int   LabelVisibility;
  int   TitleVisibility;
  
  int   LastPoint1[2];
  int   LastPoint2[2];
  
  int   LastSize[2];
  int   LastTitleFontSize;
  int   LastLabelFontSize;
  
  virtual void BuildAxis(vtkViewport *viewport);
  static float ComputeStringOffset(float width, float height, float theta);
  static void SetOffsetPosition(float xTick[3], float theta, int stringHeight,
                                int stringWidth, int offset, vtkActor2D *actor);
  virtual void UpdateAdjustedRange();

  vtkTextMapper *TitleMapper;
  vtkActor2D    *TitleActor;

  vtkTextMapper **LabelMappers;
  vtkActor2D    **LabelActors;

  vtkPolyData         *Axis;
  vtkPolyDataMapper2D *AxisMapper;
  vtkActor2D          *AxisActor;

  vtkTimeStamp  AdjustedRangeBuildTime;
  vtkTimeStamp  BuildTime;

private:
  vtkAxisActor2D(const vtkAxisActor2D&);  // Not implemented.
  void operator=(const vtkAxisActor2D&);  // Not implemented.
};


#endif
