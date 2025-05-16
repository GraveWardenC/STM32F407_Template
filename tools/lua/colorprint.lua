local colorprint = {print,printf}

local colors = {
    red     = "\027[31m",
    green   = "\027[32m",
    yellow  = "\027[33m",
    blue    = "\027[34m",
    magenta = "\027[35m",
    cyan    = "\027[36m",
    white   = "\027[37m",
    reset   = "\027[0m"
}

-- 简单打印彩色文本
function colorprint.print(color, text)
    local code = colors[color] or colors.reset
    print(code .. text .. colors.reset)
end

-- 格式化打印（支持 printf 风格）
function colorprint.printf(color, fmt, ...)
    local code = colors[color] or colors.reset
    local msg = string.format(fmt, ...)
    print(code .. msg .. colors.reset)
end

return colorprint