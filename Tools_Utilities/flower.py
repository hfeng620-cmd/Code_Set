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
def draw_rose(x, y, surface):
    # 简化绘制玫瑰轮廓，实际可使用更精细的图形或导入图片
    rose_surface = pygame.Surface((100, 100), pygame.SRCALPHA)
    pygame.draw.circle(rose_surface, (150, 200, 255, 180), (50, 50), 30)
    pygame.draw.ellipse(rose_surface, (150, 200, 255, 150), (20, 30, 60, 40))
    pygame.draw.ellipse(rose_surface, (150, 200, 255, 150), (30, 20, 40, 60))
    surface.blit(rose_surface, (x - 50, y - 50))

    # 绘制环绕的光环
    for i in range(360):
        angle = math.radians(i)
        radius = 60 + math.sin(math.radians(i * 5)) * 5
        px = x + math.cos(angle) * radius
        py = y + math.sin(angle) * radius
        pygame.draw.circle(surface, (180, 220, 255, 50), (px, py), 1)

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
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill((0, 0, 20))  # 黑色背景

    # 绘制代码
    font = pygame.font.SysFont("consolas", 14)
    for i, line in enumerate(code_text):
        text_surface = font.render(line, True, (0, 255, 0))
        screen.blit(text_surface, (20, 20 + i * 20))

    # 绘制冰晶玫瑰
    rose_x, rose_y = width // 2, height // 2
    draw_rose(rose_x, rose_y, screen)

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
