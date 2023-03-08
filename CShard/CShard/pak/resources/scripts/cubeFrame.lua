cube = obj:component(0)
cubeTrans = cube.transform
newRot = cubeTrans.rot

newY = (newRot.y + 300 * getdt())

if (newY >= 180) then
    newY = -180
end

newRot.y = newY
cubeTrans.rot = newRot 