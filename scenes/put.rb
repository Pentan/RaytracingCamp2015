mats = ["body1", "body2", "body3", "body4", "body5", "body6", "body7", "body8", "body9"]

D = 0.3
NUM = 4
JITTER = 0.2

pmax = D * NUM
puts("<!-- N:#{NUM},D:#{D},max:#{pmax} -->")

for j in -NUM..NUM
    for i in -NUM..NUM
        r = (rand - 0.5) * Math::PI * 1.0
        m = mats.sample
        x = D * (i + (rand - 0.5) * JITTER)
        z = D * (j + (rand - 0.5) * JITTER)
        #puts("x:#{x},z:#{z},#{m},#{r}")
        
        if i == 0
            if j == 0
                r = 0
                m = mats[0]
            elsif j > 0
                next
            end
        end
        
        puts("<object>")
        puts("    <transform>")
        puts("        <translate value=\"#{x}, 0.0, #{z}\"/>")
        puts("        <rotate axis=\"0.0, 1.0, 0.0\" angle=\"#{r}\"/>")
        puts("    </transform>")
        puts("    <geometry ref=\"CatMesh\"/>")
        puts("    <material ref=\"#{m}\"/>")
        puts("</object>")
        
        
    end
end
