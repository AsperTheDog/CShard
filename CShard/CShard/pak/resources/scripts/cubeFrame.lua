newRot = cubeTrans.rot

newR = (newRot.y + 300 * getdt())

if (newR >= 180) then
    newR = -180
end

newRot.y = newR
cubeTrans.rot = newRot 