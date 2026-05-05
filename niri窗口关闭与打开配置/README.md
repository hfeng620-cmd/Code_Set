# niri 窗口打开与关闭动画配置说明

这份说明记录的是我当前在这台机器上给 `niri` 配的窗口打开/关闭动画。

当前实际生效的配置文件：

```text
/home/tian/.config/niri/config.kdl
```

当前机器上的 `niri` 版本：

```text
niri 25.11-2
```

## 现在正在用的效果

- 打开窗口：像素块逐步展开
- 关闭窗口：窗口快速下坠并带随机旋转

我保留了你喜欢的打开动画，只把关闭动画换成了一个更明显的版本。

## 配置写在哪里

配置在这个文件的 `animations` 块里：

```text
/home/tian/.config/niri/config.kdl
```

其中主要是这两个节点：

- `window-open`
- `window-close`

## 当前完整配置

下面这段就是当前正在用的完整动画配置，可以直接抄回去：

```kdl
animations {
    workspace-switch {
        spring damping-ratio=0.80 stiffness=523 epsilon=0.0001
    }
    window-open {
        duration-ms 500
        curve "linear"
        custom-shader r"
            vec4 open_color(vec3 coords_geo, vec3 size_geo) {
                if (coords_geo.x < 0.0 || coords_geo.x > 1.0 ||
                        coords_geo.y < 0.0 || coords_geo.y > 1.0)
                    return vec4(0.0);

                float cell_size = 32.0;
                vec2 grid = floor(size_geo.xy / cell_size);
                grid = max(grid, vec2(1.0));

                vec2 cell = floor(coords_geo.xy * grid);
                vec2 local = fract(coords_geo.xy * grid);

                float t = (cell.x / grid.x + cell.y / grid.y) / 2.0;
                float d = 0.4;
                float cell_progress = clamp((niri_clamped_progress - t * (1.0 - d)) / d, 0.0, 1.0);

                vec2 dist = abs(local - 0.5);
                if (dist.x > cell_progress * 0.5 || dist.y > cell_progress * 0.5)
                    return vec4(0.0);

                vec2 centre = (cell + 0.5) / grid;
                vec2 sample_geo = mix(centre, coords_geo.xy, cell_progress);
                vec3 coords_tex = niri_geo_to_tex * vec3(clamp(sample_geo, 0.0, 1.0), 1.0);
                vec4 color = texture2D(niri_tex, coords_tex.st);
                return color;
            }
        "
    }
    window-close {
        duration-ms 300
        curve "linear"
        custom-shader r"
            vec4 close_color(vec3 coords_geo, vec3 size_geo) {
                float progress = niri_clamped_progress * niri_clamped_progress;
                vec2 coords = (coords_geo.xy - vec2(0.5, 1.0)) * size_geo.xy;

                coords.y -= progress * 1440.0;

                float random = (niri_random_seed - 0.5) / 2.0;
                random = sign(random) - random;
                float max_angle = 0.5 * random;

                float angle = progress * max_angle;
                mat2 rotate = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
                coords = rotate * coords;

                coords_geo = vec3(coords / size_geo.xy + vec2(0.5, 1.0), 1.0);

                vec3 coords_tex = niri_geo_to_tex * coords_geo;
                vec4 color = texture2D(niri_tex, coords_tex.st);
                return color;
            }
        "
    }
}
```

## 这些配置是从哪里找到的

### 1. 打开窗口动画来源

打开动画来自 GitHub Gist 的一个 `niri` 像素动画示例：

```text
https://gist.github.com/JPDye/ca55c7f53dd31ae7f42d83d28feae570
```

这个版本的特点是：

- 不是简单淡入淡出
- 窗口会按像素块逐步展开
- 视觉上比较明显
- 你前面试过之后表示比较喜欢这个打开效果

### 2. 关闭窗口动画来源

关闭动画参考的是另一个 GitHub Gist 里的更激进版本：

```text
https://gist.github.com/fabienjuif/f27c89196782e6ef7b270d93c492d897
```

我现在用的是它的思路：

- 关闭时不是简单透明消失
- 而是先快速向下坠落
- 同时带随机角度旋转
- 这样肉眼更容易看见“窗口正在关闭”

### 3. 官方文档参考

除了这些 GitHub 例子，我还参考了 `niri` 官方动画配置文档：

```text
https://github.com/niri-wm/niri/wiki/Configuration:-Animations
```

## 我实际改了什么

我只改了：

```text
/home/tian/.config/niri/config.kdl
```

没有新建额外的 `.frag` 文件，而是直接把 shader 写进 `config.kdl` 的 `custom-shader` 里。

这样做的好处是：

- 配置集中
- 以后你自己改更方便
- 备份时只需要备份一个文件

## 我是怎么应用配置的

先验证配置语法：

```bash
niri validate -c /home/tian/.config/niri/config.kdl
```

再热重载到当前 `niri` 会话：

```bash
niri msg action load-config-file
```

## 如果你以后想继续调

### 调打开动画

位置：

```text
/home/tian/.config/niri/config.kdl
```

重点看这些值：

- `duration-ms 500`
- `float cell_size = 32.0;`
- `float d = 0.4;`

含义大致是：

- `duration-ms` 越大，动画越慢
- `cell_size` 越大，块感越粗
- `d` 越大，单个块展开得越慢

### 调关闭动画

位置：

```text
/home/tian/.config/niri/config.kdl
```

重点看这些值：

- `duration-ms 300`
- `coords.y -= progress * 1440.0;`
- `float max_angle = 0.5 * random;`

含义大致是：

- `duration-ms` 越大，关窗动作越拖
- `1440.0` 越大，下坠越猛
- `0.5` 越大，旋转越夸张

## 如果你想回退成最普通的版本

你可以把 `window-open` 和 `window-close` 改回这种基础配置：

```kdl
window-open {
    duration-ms 150
    curve "ease-out-expo"
}

window-close {
    duration-ms 150
    curve "ease-out-quad"
}
```

然后再执行：

```bash
niri validate -c /home/tian/.config/niri/config.kdl
niri msg action load-config-file
```

## 结论

现在这台机器上的方案是：

- 打开：GitHub 上的 pixelated 版本
- 关闭：GitHub 上更明显的 fall-and-rotate 版本

如果你后面还想换，我建议继续保持这个思路：

- 打开动画用你喜欢的 pixelated
- 关闭动画单独换

这样比较容易试，不会两个动画一起变得太乱。
