// Assemble the front panel.



include <BoxCommon.scad>
use <MK_Panel.scad>
use <Bezel.scad>
use <DMM_Front_Block.scad>


outsideR=10;
// The dimensions of the front most rounded corner cube
panelDim=[DISPBLOCK.x,DISPBLOCK.y,0]+[40,100,3.2];
// The holder is slightly bigger
dispHolder=DISPBLOCK+[20,20,2];
dispCenter=[0,30,panelDim.z];
DMM_Center=[0,-35,0];

dispCut=[51*3,23,panelDim.z];

difference()
{
    union()
    {
        dressPanel(panelDim,8, outsideR,outsideR-3.2,outsideR-5.5);
        translate(dispCenter+[0,0,dispHolder.z/2] ) cube(dispHolder,center=true);
        translate(DMM_Center) DMM_Front_U();
        
    }
    // Turn the following statement on to see a cross section
    *translate([0,-50,-1])cube(100,100,10);
    translate(dispCenter) cubeXY(DISPBLOCK+[1,1,15]);
    translate([dispCenter.x,dispCenter.y,-.1])
        Bezel(dispCut+[1,1,.2]);
    translate(DMM_Center)  DMM_Front_D();
}
