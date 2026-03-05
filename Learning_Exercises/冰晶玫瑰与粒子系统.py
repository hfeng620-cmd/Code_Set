import pygame
import random
import math

# 初始化pygame
pygame.init()
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("冰晶玫瑰与代码")
clock = pygame.time.Clock()

# 定义粒子类
class Particle:
    def __init__(self, x, y, color, size, speed, angle):
        self.x = x
        self.y = y
        self.color = color
        self.size = size
        self.speed = speed
        self.angle = angle
        self.vx = math.cos(angle) * speed
        self.vy = math.sin(angle) * speed
        self.life = 255  # 粒子生命周期（透明度）

    def update(self):
        self.x += self.vx
        self.y += self.vy
        self.life -= 1  # 生命周期减少
        # 粒子大小随生命周期变化
        self.size = max(1, self.size * 0.98)

    def draw(self, surface):
        if self.life > 0:
            # 设置透明度
            alpha_surface = pygame.Surface((self.size * 2, self.size * 2), pygame.SRCALPHA)
            pygame.draw.circle(alpha_surface, (*self.color, self.life), (self.size, self.size), self.size)
            surface.blit(alpha_surface, (self.x - self.size, self.y - self.size))

# 玫瑰绘制函数
def draw_rose(x, y, surface, angle_offset=0):
    # 绘制真正的玫瑰花瓣，同时添加冰晶光效
    color_petal = (200, 150, 255)  # 花瓣颜色
    color_highlight = (255, 200, 255)  # 高光
    color_crystal = (150, 200, 255)  # 冰晶颜色
    
    # 绘制多层花瓣
    layers = 3
    petals_per_layer = 5
    for layer in range(layers):
        radius = 20 + layer * 15
        for petal in range(petals_per_layer):
            angle = petal * (360 / petals_per_layer) + angle_offset + layer * 20
            rad_angle = math.radians(angle)
            
            # 花瓣位置
            petal_x = x + radius * math.cos(rad_angle)
            petal_y = y + radius * math.sin(rad_angle)
            
            # 绘制花瓣（椭圆）
            petal_surface = pygame.Surface((40, 20), pygame.SRCALPHA)
            pygame.draw.ellipse(petal_surface, (*color_petal, 200), (0, 0, 40, 20))
            # 旋转花瓣
            rotated_petal = pygame.transform.rotate(petal_surface, -angle)
            petal_rect = rotated_petal.get_rect(center=(petal_x, petal_y))
            surface.blit(rotated_petal, petal_rect)
            
            # 添加冰晶高光
            highlight_x = petal_x + 10 * math.cos(rad_angle)
            highlight_y = petal_y + 10 * math.sin(rad_angle)
            pygame.draw.circle(surface, color_highlight, (int(highlight_x), int(highlight_y)), 2)
    
    # 中心
    pygame.draw.circle(surface, color_crystal, (x, y), 8)
    
    # 添加冰晶枝条
    for i in range(8):
        angle = i * 45 + angle_offset
        rad_angle = math.radians(angle)
        end_x = x + 70 * math.cos(rad_angle)
        end_y = y + 70 * math.sin(rad_angle)
        pygame.draw.line(surface, color_crystal, (x, y), (end_x, end_y), 1)
        # 小晶体
        pygame.draw.polygon(surface, color_crystal, [(end_x, end_y-4), (end_x-3, end_y+3), (end_x+3, end_y+3)])
    
    # 绘制花柄
    stem_color = (100, 150, 100)  # 绿色花柄
    pygame.draw.line(surface, stem_color, (x, y + 50), (x, y + 150), 4)
    
    # 绘制叶子
    leaf_color = (50, 100, 50)
    # 左叶子
    pygame.draw.ellipse(surface, leaf_color, (x - 30, y + 80, 40, 20))
    pygame.draw.line(surface, (0, 50, 0), (x - 10, y + 90), (x - 40, y + 85), 2)  # 叶脉
    # 右叶子
    pygame.draw.ellipse(surface, leaf_color, (x - 10, y + 100, 40, 20))
    pygame.draw.line(surface, (0, 50, 0), (x + 10, y + 110), (x + 40, y + 105), 2)  # 叶脉
    
    # 绘制环绕的光环（需要在支持 alpha 的图层上绘制，再 blit 到主 surface）
    halo_surface = pygame.Surface(surface.get_size(), pygame.SRCALPHA)
    for i in range(360):
        angle = math.radians(i)
        radius = 80 + math.sin(math.radians(i * 6)) * 10
        px = x + math.cos(angle) * radius
        py = y + math.sin(angle) * radius
        pygame.draw.circle(halo_surface, (180, 220, 255, 100), (int(px), int(py)), 1)
    surface.blit(halo_surface, (0, 0))

# 代码文本
code_text = [
    "function particleCompareFunc(p1, p2) {",
    "  if (p1.y < p2.y) return -1;",
    "  else if (p1.y > p2.y) return 1;",
    "  return 0;",
    "}",
    "class Plugin {",
    "  constructor() {",
    "    this.defaults = {",
    "      responsive: true,",
    "      maxParticles: 150,",
    "      sizeVariations: 3,",
    "      speed: 0.5,",
    "      color: '#00ffff',",
    "      minDistance: 120,",
    "      connectParticles: false",
    "    };",
    "  }",
    "  init(settings) {",
    "    this.options = { ...this.defaults, ...settings };",
    "  }",
    "}"
]

particles = []
angle_offset = 0
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    angle_offset += 0.5  # 旋转角度

    screen.fill((0, 0, 20))  # 黑色背景

    # 绘制代码
    font = pygame.font.SysFont("consolas", 14)
    for i, line in enumerate(code_text):
        text_surface = font.render(line, True, (0, 255, 0))
        screen.blit(text_surface, (20, 20 + i * 20))

    # 绘制冰晶玫瑰
    rose_x, rose_y = width // 2, height // 2
    draw_rose(rose_x, rose_y, screen, angle_offset)

    # 生成粒子
    if random.random() < 0.5:  # 控制粒子生成频率
        angle = random.uniform(0, math.pi * 2)
        speed = random.uniform(1, 3)
        size = random.uniform(2, 5)
        color = (150, 200, 255)
        particle = Particle(rose_x, rose_y, color, size, speed, angle)
        particles.append(particle)

    # 更新并绘制粒子
    for particle in particles[:]:
        particle.update()
        particle.draw(screen)
        if particle.life <= 0:
            particles.remove(particle)

    pygame.display.flip()
    clock.tick(60)

pygame.quit()