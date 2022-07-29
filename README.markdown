# lua binding to [libbloom](https://github.com/jvirkki/libbloom)

```lua
local bloom = require("bloom")

local b = bloom.new(1000000, 0.01)
b:add("aaaaaa")
b:add("bbbbb")

if b:check("bbbbb") then
    print("It may be there!\n")
end

if b:check("a") then
    print("It not be there!\n")
end

b:add("a")

if b:check("a") then
    print("It now be there!\n")
end
```