// Assemble the front panel.



include <BoxCommon.scad>
use <MK_Panel.scad>
use <Bezel.scad>
use <DMM_Front_Block.scad>
//DMM_Center=[50,7,0];
$fs=.1;

outsideR=10;
// The dimensions of the front most rounded corner cube
panelDim=[DISPBLOCK.x,DISPBLOCK.y,0]+[20,70,3.2];
innerPanelDim=[panelDim.x-2*outsideR,panelDim.y-2*outsideR,panelDim.z];

dmmSupportSize=[70,20,6+9];

DMM_Center=align("SE",innerPanelDim)+[-dmmSupportSize.x/2,dmmSupportSize.y*2+6,0];

SPK_DIA=12;
SPK_HT=9;
SPK_Port_DIA=2.5;
SPK_HOLDER_DIA=SPK_DIA+4;

switchSz=[14,13,2];

SPK_Center=align("W",panelDim,center=true)+ [SPK_HOLDER_DIA/2+25,3-SPK_HOLDER_DIA/2,0];
// The holder is slightly bigger
dispHolder=DISPBLOCK+[5,5,2];
dispCenter=[0,-8-DISPBLOCK.y/2+panelDim.y/2,panelDim.z];

// The bottom of the display
dispBottom=dispCenter+[0,-dispHolder.y/2,0];
*translate(dispBottom)
    cube([100,1,100],center=true);  // Show the location

// The hole in the front with the bezel
dispCut=[51*3,21,panelDim.z];
lipHt=10;

difference()
{
    union()
    {
        dressPanel(panelDim,lipHt, outsideR,outsideR-3.2,outsideR-5.5);
        // The holder for the display
        translate(dispCenter+[0,0,dispHolder.z/2] ) cube(dispHolder,center=true);
        // The holder for the DMM
        translate(DMM_Center) DMM_Front_U();
        // Block for the speaker
        translate(SPK_Center+[0,0,3]) cylinder(d=SPK_HOLDER_DIA,h=SPK_HT);
        
    }
    // Turn the following statement on to see a cross section
    *translate([0,-50,-1])cube(100,100,10);
    translate(dispCenter) cubeXY(DISPBLOCK+[1.5,1.5,15]);
    translate([dispCenter.x,dispCenter.y,-.1])
        Bezel(dispCut+[1,1,.2]);
    translate(DMM_Center)  DMM_Front_D();
    
    // Insert holes
    #translate([panelDim.x/2,0,panelDim.z+lipHt/2]) rotate([0,90,0]) cylinder(h=25,d=5.5,center=true);
    #translate([-panelDim.x/2,0,panelDim.z+lipHt/2]) rotate([0,90,0]) cylinder(h=25,d=5.5,center=true);
    
    // Draw the knob hole
    translate([-panelDim.x/4,4+-panelDim.y/4,0])
        cylinder(h=25,d=7.5,center=true);
    translate([-panelDim.x/4,4+-panelDim.y/4,panelDim.z])
        cube(switchSz,center=true);

    // Finish the speaker
    translate(SPK_Center+[0,0,2])
    {
        cylinder(d=SPK_DIA,h=SPK_HT+5);
        cylinder(d=SPK_Port_DIA,h=25,center=true);
    }
}
