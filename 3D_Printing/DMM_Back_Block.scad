include <BoxCommon.scad>;
$fs=.01;
// This is going to look upside down because of
// coordinate system for the board is from the front.
// Whereas this view is from the back. Its all
// flipped on the x axis.


BlockSZ=[54+10,8,5.5]; //  Of the board holders

// Locate the connectors on the back of the board
BRD_Center_X=53.34/2; // from either edge.
PWR_Center_X=BRD_Center_X-8.38; // Offset from center
USB_Center_X=BRD_Center_X-38.1;  // to center

USB_SZ=[13,12,50];
PWR_SZ=[10,12,50];



// Use these modules with the same translate parameters
module DMM_Back_U() // Really, just the cpu.
{  

    
   translate([0,-BlockSZ.y/2,BlockSZ.z/2]) cube(BlockSZ,center=true);
   
}
module DMM_Back_D()
{
    translate([0,0,panelDim.z])
    {
                     // The board is centered on 0,0 and 0 above z
        translate([0,-4,20/2])
         {
             cube([CPU_BOARD_X, CPU_BOARD_Y,20],center=true);
             translate([USB_Center_X,-USB_SZ.y/2,0])
                cube(USB_SZ,center=true);
             translate([PWR_Center_X,-PWR_SZ.y/2,0])
                cube(PWR_SZ,center=true);         }
      }

    
}
difference()
{
     union(){
        cylinder(r=1,h=50);
        DMM_Back_U();
     }
    DMM_Back_D();
    translate([0,-50,0]) cube([50,50,30]);
}