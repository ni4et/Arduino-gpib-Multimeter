
include <BoxCommon.scad>
use <MK_Panel.scad>
use <Bezel.scad>
use <DMM_Front_Block.scad>

$fs=.1;

outerShell(panelDim+[0,0,10], outsideR,outsideR-2.8);