S = "\s"
T = "\t"
L = "\n"

$cmd_map = {
  "push" => [S, S],
  "duplicate" => [S, L, S],
  "copy" => [S, T, S],
  "swap" => [S, L, T],
  "discard" => [S, L, L],
  "slide" => [S, T, L],
  
  "add" => [T, S, S, S],
  "sub" => [T, S, S, T],
  "mul" => [T, S, S, L],
  "div" => [T, S, T, S],
  "mod" => [T, S, T, T],

  "store" => [T, T, S],
  "retrieve" => [T, T, T],

  "mark" => [L, S, S],
  "call" => [L, S, T],
  "jump" => [L, S, L],
  "jump-if-zero" => [L, T, S],
  "jump-if-negative" => [L, T, T],
  "return" => [L, T, L],
  "end" => [L, L, L],

  "putc" => [T, L, S, S],
  "putn" => [T, L, S, T],
  "readc" => [T, L, T, S],
  "readn" => [T, L, T, T]
}

def to_ws_num num
  num = num.to_i unless num === Numeric
  sign = if num < 0
    "1"
  else
    "0"
  end

  (sign + num.abs.to_s(2)).gsub("0", S).gsub("1", T) + L
end

def to_ws_cmd cmd
  $cmd_map[cmd].join
end

$label_map = {}

def to_ws_label param
  if $label_map[param]
    i = $label_map[param]
    to_ws_num(i)
  else
    i = $label_map.keys.size + 1
    $label_map[param] = i
    to_ws_num(i)
  end
end

def to_ws_param cmd, param
  if cmd =~ /mark|call|jump/
    to_ws_label(param)
  else
    to_ws_num(param)
  end
end

while line = gets
  arr = line.split(/;/)
  part = arr.first
  part.chomp!
  part.strip!
  next if part.length == 0
  cmd, param = part.split(/\s/)
  cmd_s = to_ws_cmd(cmd)
  if param
    num_s = to_ws_param(cmd, param)
    print cmd_s + num_s
  else
    print cmd_s
  end
end
