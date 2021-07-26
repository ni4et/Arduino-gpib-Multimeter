// Assemble the front panel.



include <BoxCommon.scad>
use <MK_Panel.scad>
use <Bezel.scad>
use <DMM_Front_Block.scad>
//DMM_Center=[50,7,0];
$fs=.1;

outsideR=10;
// The dimensions of the front most rounded corner cube
panelDim=[DISPBLOCK.x,DISPBLOCK.y,0]+[30,85,3.2];
innerPanelDim=[panelDim.x-2*outsideR,panelDim.y-2*outsideR,panelDim.z];

dmmSupportSize=[70,20,6+9];

DMM_Center=align("SE",innerPanelDim)+[-dmmSupportSize.x/2,dmmSupportSize.y*2+6,0];
// The holder is slightly bigger
dispHolder=DISPBLOCK+[20,20,2];
dispCenter=[0,-15-DISPBLOCK.y/2+panelDim.y/2,panelDim.z];

// The bottom of the display
dispBottom=dispCenter+[0,-dispHolder.y/2,0];
*translate(dispBottom)
    cube([100,1,100],center=true);  // Show the location

dispCut=[51*3,23,panelDim.z];

difference()
{
    union()
    {
        dressPanel(panelDim,10, outsideR,outsideR-3.2,outsideR-5.5);
        // The holder for the display
        translate(dispCenter+[0,0,dispHolder.z/2] ) cube(dispHolder,center=true);
        // The holder for the DMM
        translate(DMM_Center) DMM_Front_U();
        
    }
    // Turn the following statement on to see a cross section
    *translate([0,-50,-1])cube(100,100,10);
    translate(dispCenter) cubeXY(DISPBLOCK+[1,1,15]);
    translate([dispCenter.x,dispCenter.y,-.1])
        Bezel(dispCut+[1,1,.2]);
    translate(DMM_Center)  DMM_Front_D();
    
    // Insert holes
    #translate([panelDim.x/2,0,panelDim.z+8/2]) rotate([0,90,0]) cylinder(h=15,d=5.5,center=true);
    #translate([-panelDim.x/2,0,panelDim.z+8/2]) rotate([0,90,0]) cylinder(h=15,d=5.5,center=true);
    
    // Draw the knob hole
    translate([-panelDim.x/4,4+-panelDim.y/4,0])
        cylinder(h=25,d=7.5,center=true);
}
