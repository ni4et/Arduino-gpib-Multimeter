
include <BoxCommon.scad>
use <MK_Panel.scad>
use <Bezel.scad>
use <DMM_Front_Block.scad>


h=114.6-3.2;  // Measure!!!!

$fs=.025;

difference()
{
    // minkowski adds the box and cylinder heights.
    outerShell(panelDim-[0,0,panelDim.z]+[0,0,h], outsideR,outsideR-2.8);
    #translate([0,0,lipHt/2]) rotate([0,-90,0]) cylinder(h=panelDim.x+10,d=4.5,center=true);
    #translate([0,0,h-lipHt/2]) rotate([0,-90,0]) cylinder(h=panelDim.x+10,d=4.5,center=true);
}