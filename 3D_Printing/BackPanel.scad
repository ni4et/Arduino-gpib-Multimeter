// Assemble the front panel.



include <BoxCommon.scad>
use <MK_Panel.scad>
use <Bezel.scad>
use <DMM_Back_Block.scad>
//DMM_Center=[50,7,0];
$fs=.02;


// The dimensions of the front most rounded corner cube

innerPanelDim=[panelDim.x-2*outsideR,panelDim.y-2*outsideR,panelDim.z];

dmmSupportSize=[70,20,6+9];

DMM_Center=align("SE",innerPanelDim)+[-15-dmmSupportSize.x/2,dmmSupportSize.y*2+6,0];
DMM_B_Center=[-DMM_Center.x, DMM_Center.y, DMM_Center.z];
SPK_DIA=12.4;
SPK_HT=9;
SPK_Port_DIA=2.5;
SPK_HOLDER_DIA=SPK_DIA+4;

switchCenter=[-panelDim.x/4,4+-panelDim.y/4,0];
switchSz=[14,13,2];

//SPK_Center=align("W",panelDim,center=true)+ [SPK_HOLDER_DIA/2+25,3-SPK_HOLDER_DIA/2,0];
SPK_Center=[10-panelDim.x/2,switchCenter.y,0] + [SPK_HOLDER_DIA/2,0,0];
// The holder is slightly bigger
dispHolder=DISPBLOCK+[5,5,3];
dispCenter=[0,-8-DISPBLOCK.y/2+panelDim.y/2,panelDim.z];

// The bottom of the display
dispBottom=dispCenter+[0,-dispHolder.y/2,0];
*translate(dispBottom)
    cube([100,1,100],center=true);  // Show the location

// The hole in the front with the bezel
dispCut=[51*3,21,panelDim.z];



difference()
{
    union()
    {
    
        // This marks the center of the mega2560
        * translate(DMM_B_Center+[0,-4,0])color(c=[0,1,1,1]) cylinder(d=1,h=100,center=false);
        
        dressPanel(panelDim,lipHt, outsideR,outsideR-3.2,outsideR-5.5);
        color("cyan") translate(DMM_B_Center) DMM_Back_U();
        // Blocks for the screws
        translate([panelDim.x/2-10,0,panelDim.z+lipHt/2]) cube([10,10,lipHt],center=true);
        translate([-panelDim.x/2+10,0,panelDim.z+lipHt/2]) cube([10,10,lipHt],center=true);
    }
    // Turn the following statement on to see a cross section
    *translate([80,-15,-1])cube(100,100,10);
    
    
    // cut the slots for the boards.
    translate(DMM_B_Center)  DMM_Back_D();
    
    // Insert holes for the screws
    #translate([panelDim.x/2,0,panelDim.z+lipHt/2]) rotate([0,-90,0]) cylinder(h=15,d=5.5,center=false);
    #translate([-panelDim.x/2,0,panelDim.z+lipHt/2]) rotate([0,90,0]) cylinder(h=15,d=5.5,center=false);
    
   }
