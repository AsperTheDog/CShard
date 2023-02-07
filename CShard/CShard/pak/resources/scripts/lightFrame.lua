newColor = li.color
newPos = trans.pos
if (not bouncing) then
    newxC = (newColor.x + 1 * getdt())
    newxP = (newPos.z + 4 * getdt())
else
    newxC = (newColor.x - 1 * getdt())
    newxP = (newPos.z - 4 * getdt())
end

if (newxC > 1.0) then
    bouncing = true
elseif (newxC < 0.0) then
    bouncing = false
end
newColor.x = newxC
fade.intensity = newxC
grain.intensity = (1 - newxC) * 0.5
newPos.z = newxP
li.color = newColor
trans.pos = newPos

cam:anchor(trans.pos)