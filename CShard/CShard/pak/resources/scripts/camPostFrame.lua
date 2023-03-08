camPos = cam.pos

if camPos.z >= 24 then
    att1 = 1 - clamp((camPos.z - 24), 0, 1)
elseif camPos.z <= 6 then
    att1 = 1 - clamp((6 - camPos.z), 0, 1)
    if att1 == 0 then
        cam.pos = initialPos
    end
end

if camPos.z >= 18 then
    att2 = clamp((1 - (camPos.z - 18)), 0, 1)
end

fade2.intensity = att1
fade1.intensity = att2