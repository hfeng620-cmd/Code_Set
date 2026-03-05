"""
B站视频内容爬取与总结工具
用于提取B站视频信息并生成内容总结
"""
from DrissionPage import ChromiumPage
import time
import json
from datetime import datetime
import re
from typing import Optional, Dict, Any


def fetch_bilibili_video_info(url: str) -> Optional[Dict[str, Any]]:
    """
    获取B站视频页面内容并提取视频信息
    
    Args:
        url: B站视频链接
        
    Returns:
        dict: 包含提取视频信息的字典，失败时返回None
    """
    print(f"正在访问B站视频页面: {url}")
    
    # 创建浏览器页面
    page = ChromiumPage()
    
    try:
        # 访问页面
        page.get(url)
        time.sleep(8)  # 等待页面完全加载，B站页面需要较长时间
        
        print(f"页面标题: {page.title}")
        
        # 获取页面HTML
        html = page.html
        
        # 保存原始HTML以便调试
        with open('bilibili_raw.html', 'w', encoding='utf-8') as f:
            f.write(html)
        print("已保存原始页面到 bilibili_raw.html")
        
        # 提取视频标题
        video_title = ""
        title_selectors = [
            'h1.video-title',
            '.video-title',
            '.video-info .title',
            'h1[itemprop="name"]',
            '.bpx-player-primary-area h1',
            'h1'
        ]
        
        for selector in title_selectors:
            elements = page.eles(selector)
            if elements:
                video_title = elements[0].text.strip()
                print(f"找到视频标题: {video_title}")
                break
        
        # 提取UP主信息
        up_author = ""
        up_selectors = [
            '.up-info .username',
            '.up-name',
            '.up-info .name',
            '.video-info .up-name',
            '.up-card .name'
        ]
        
        for selector in up_selectors:
            elements = page.eles(selector)
            if elements:
                up_author = elements[0].text.strip()
                print(f"找到UP主: {up_author}")
                break
        
        # 提取播放量
        play_count = ""
        play_selectors = [
            '.video-info .view',
            '.view-item .view',
            '.video-data .view',
            '.bpx-player-video-info .view'
        ]
        
        for selector in play_selectors:
            elements = page.eles(selector)
            if elements:
                play_count = elements[0].text.strip()
                print(f"找到播放量: {play_count}")
                break
        
        # 提取弹幕数
        danmaku_count = ""
        danmaku_selectors = [
            '.video-info .dm',
            '.view-item .dm',
            '.video-data .dm'
        ]
        
        for selector in danmaku_selectors:
            elements = page.eles(selector)
            if elements:
                danmaku_count = elements[0].text.strip()
                print(f"找到弹幕数: {danmaku_count}")
                break
        
        # 提取点赞数
        like_count = ""
        like_selectors = [
            '.video-info .like',
            '.ops .like',
            '.toolbar .like'
        ]
        
        for selector in like_selectors:
            elements = page.eles(selector)
            if elements:
                like_count = elements[0].text.strip()
                print(f"找到点赞数: {like_count}")
                break
        
        # 提取投币数
        coin_count = ""
        coin_selectors = [
            '.video-info .coin',
            '.ops .coin'
        ]
        
        for selector in coin_selectors:
            elements = page.eles(selector)
            if elements:
                coin_count = elements[0].text.strip()
                print(f"找到投币数: {coin_count}")
                break
        
        # 提取收藏数
        favorite_count = ""
        favorite_selectors = [
            '.video-info .collect',
            '.ops .collect'
        ]
        
        for selector in favorite_selectors:
            elements = page.eles(selector)
            if elements:
                favorite_count = elements[0].text.strip()
                print(f"找到收藏数: {favorite_count}")
                break
        
        # 提取发布时间
        publish_time = ""
        time_selectors = [
            '.video-info .publish',
            '.video-data .publish',
            '.video-info .time'
        ]
        
        for selector in time_selectors:
            elements = page.eles(selector)
            if elements:
                publish_time = elements[0].text.strip()
                print(f"找到发布时间: {publish_time}")
                break
        
        # 提取视频描述/简介
        video_description = ""
        desc_selectors = [
            '.video-desc',
            '.desc-info',
            '.video-info .desc',
            '.introduction'
        ]
        
        for selector in desc_selectors:
            elements = page.eles(selector)
            if elements:
                video_description = elements[0].text.strip()
                print(f"找到视频描述，长度: {len(video_description)} 字符")
                break
        
        # 提取视频标签
        video_tags = []
        tag_selectors = [
            '.tag-area .tag',
            '.video-tags .tag',
            '.tags .tag'
        ]
        
        for selector in tag_selectors:
            elements = page.eles(selector)
            if elements:
                video_tags = [tag.text.strip() for tag in elements]
                print(f"找到 {len(video_tags)} 个标签: {', '.join(video_tags[:5])}")
                break
        
        # 提取视频分区
        video_category = ""
        category_selectors = [
            '.video-info .category',
            '.crumb .item',
            '.nav-item.active'
        ]
        
        for selector in category_selectors:
            elements = page.eles(selector)
            if elements:
                video_category = elements[0].text.strip()
                print(f"找到视频分区: {video_category}")
                break
        
        # 尝试提取视频时长
        video_duration = ""
        duration_selectors = [
            '.bpx-player-ctrl-time-duration',
            '.duration',
            '.video-duration'
        ]
        
        for selector in duration_selectors:
            elements = page.eles(selector)
            if elements:
                video_duration = elements[0].text.strip()
                print(f"找到视频时长: {video_duration}")
                break
        
        # 构建结果
        result = {
            "url": url,
            "title": video_title,
            "author": up_author,
            "stats": {
                "play_count": play_count,
                "danmaku_count": danmaku_count,
                "like_count": like_count,
                "coin_count": coin_count,
                "favorite_count": favorite_count
            },
            "metadata": {
                "publish_time": publish_time,
                "category": video_category,
                "duration": video_duration,
                "tags": video_tags
            },
            "description": video_description,
            "fetch_time": datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        }
        
        return result
        
    except Exception as e:
        print(f"爬取过程中发生错误: {e}")
        import traceback
        traceback.print_exc()
        return None
        
    finally:
        # 关闭浏览器
        page.quit()


def generate_summary(data: Dict[str, Any]) -> str:
    """
    根据提取的数据生成内容总结
    
    Args:
        data: 提取的视频数据
        
    Returns:
        str: 生成的总结文本
    """
    if not data:
        return "未能获取视频信息"
    
    summary_parts = []
    
    # 基本信息
    summary_parts.append(f"视频标题：{data['title']}")
    summary_parts.append(f"UP主：{data['author']}")
    summary_parts.append(f"发布时间：{data['metadata']['publish_time']}")
    
    # 统计数据
    stats = data['stats']
    summary_parts.append(f"播放量：{stats['play_count']}")
    summary_parts.append(f"弹幕数：{stats['danmaku_count']}")
    summary_parts.append(f"点赞数：{stats['like_count']}")
    summary_parts.append(f"投币数：{stats['coin_count']}")
    summary_parts.append(f"收藏数：{stats['favorite_count']}")
    
    # 视频信息
    if data['metadata']['duration']:
        summary_parts.append(f"视频时长：{data['metadata']['duration']}")
    
    if data['metadata']['category']:
        summary_parts.append(f"视频分区：{data['metadata']['category']}")
    
    if data['metadata']['tags']:
        summary_parts.append(f"视频标签：{', '.join(data['metadata']['tags'][:5])}")
    
    # 视频描述
    if data['description']:
        # 截取前500个字符作为描述摘要
        desc_summary = data['description'][:500]
        if len(data['description']) > 500:
            desc_summary += "..."
        summary_parts.append(f"\n视频描述：\n{desc_summary}")
    
    # 添加分析总结
    summary_parts.append("\n=== 内容分析总结 ===")
    
    # 根据播放量判断热度
    if stats['play_count']:
        try:
            # 尝试提取数字
            play_num = int(re.sub(r'[^\d]', '', stats['play_count']))
            if play_num > 1000000:
                summary_parts.append("🔥 这是一个非常热门的视频（播放量超过100万）")
            elif play_num > 100000:
                summary_parts.append("👍 这是一个热门视频（播放量超过10万）")
            elif play_num > 10000:
                summary_parts.append("📈 这是一个有一定热度的视频")
            else:
                summary_parts.append("📊 这是一个普通热度的视频")
        except:
            summary_parts.append(f"📊 播放量：{stats['play_count']}")
    
    # 根据点赞/播放比判断质量
    if stats['like_count'] and stats['play_count']:
        try:
            like_num = int(re.sub(r'[^\d]', '', stats['like_count']))
            play_num = int(re.sub(r'[^\d]', '', stats['play_count']))
            if play_num > 0:
                like_ratio = like_num / play_num
                if like_ratio > 0.1:
                    summary_parts.append("⭐ 视频质量很高（点赞/播放比超过10%）")
                elif like_ratio > 0.05:
                    summary_parts.append("✨ 视频质量不错（点赞/播放比超过5%）")
                else:
                    summary_parts.append("📝 视频质量一般")
        except:
            pass
    
    # 根据描述长度判断内容详细程度
    if data['description']:
        desc_len = len(data['description'])
        if desc_len > 1000:
            summary_parts.append("📋 UP主提供了非常详细的视频描述")
        elif desc_len > 500:
            summary_parts.append("📝 UP主提供了较为详细的视频描述")
        elif desc_len > 100:
            summary_parts.append("📄 UP主提供了基本的视频描述")
        else:
            summary_parts.append("📌 视频描述较为简洁")
    
    # 根据标签数量判断内容分类
    if data['metadata']['tags']:
        tag_count = len(data['metadata']['tags'])
        if tag_count > 10:
            summary_parts.append("🏷️ 视频标签丰富，内容分类明确")
        elif tag_count > 5:
            summary_parts.append("🏷️ 视频有多个相关标签")
        else:
            summary_parts.append("🏷️ 视频标签较少")
    
    summary_parts.append(f"\n抓取时间：{data['fetch_time']}")
    summary_parts.append(f"原始链接：{data['url']}")
    
    return "\n".join(summary_parts)


def save_to_markdown(data: Dict[str, Any], output_path: str):
    """
    将数据保存为Markdown格式
    
    Args:
        data: 提取的数据
        output_path: 输出文件路径
    """
    if not data:
        print("没有数据可保存")
        return
    
    summary = generate_summary(data)
    
    with open(output_path, 'w', encoding='utf-8') as f:
        # 写入标题
        f.write(f"# {data['title']}\n\n")
        
        # 写入基本信息
        f.write("## 基本信息\n\n")
        f.write(f"- **UP主**: {data['author']}\n")
        f.write(f"- **发布时间**: {data['metadata']['publish_time']}\n")
        f.write(f"- **视频分区**: {data['metadata']['category']}\n")
        if data['metadata']['duration']:
            f.write(f"- **视频时长**: {data['metadata']['duration']}\n")
        f.write(f"- **抓取时间**: {data['fetch_time']}\n")
        f.write(f"- **原始链接**: [{data['url']}]({data['url']})\n\n")
        
        # 写入统计数据
        f.write("## 统计数据\n\n")
        stats = data['stats']
        f.write(f"- **播放量**: {stats['play_count']}\n")
        f.write(f"- **弹幕数**: {stats['danmaku_count']}\n")
        f.write(f"- **点赞数**: {stats['like_count']}\n")
        f.write(f"- **投币数**: {stats['coin_count']}\n")
        f.write(f"- **收藏数**: {stats['favorite_count']}\n\n")
        
        # 写入视频标签
        if data['metadata']['tags']:
            f.write("## 视频标签\n\n")
            for tag in data['metadata']['tags']:
                f.write(f"`{tag}` ")
            f.write("\n\n")
        
        # 写入视频描述
        if data['description']:
            f.write("## 视频描述\n\n")
            f.write(f"{data['description']}\n\n")
        
        # 写入内容总结
        f.write("## 内容分析总结\n\n")
        f.write(f"{summary}\n\n")
        
        # 添加分隔线
        f.write("---\n\n")
        f.write("*本文件由B站视频内容提取工具自动生成*\n")
    
    print(f"已保存到Markdown文件: {output_path}")


def save_to_html(data: Dict[str, Any], output_path: str):
    """
    将数据保存为HTML格式
    
    Args:
        data: 提取的数据
        output_path: 输出文件路径
    """
    if not data:
        print("没有数据可保存")
        return
    
    summary = generate_summary(data)
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(f"""<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>B站视频内容总结 - {data['title']}</title>
    <style>
        body {{ font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'PingFang SC', 'Hiragino Sans GB', 'Microsoft YaHei', 'Helvetica Neue', Helvetica, Arial, sans-serif; line-height: 1.6; color: #1a1a1a; max-width: 800px; margin: 0 auto; padding: 20px; background-color: #f8f9fa; }}
        h1 {{ color: #00a1d6; border-bottom: 2px solid #00a1d6; padding-bottom: 10px; }}
        h2 {{ color: #262626; margin-top: 30px; padding-bottom: 5px; border-bottom: 1px solid #e5e5e5; }}
        h3 {{ color: #595959; }}
        .meta {{ color: #8590a6; font-size: 14px; margin-bottom: 20px; background-color: #fff; padding: 15px; border-radius: 8px; border-left: 4px solid #00a1d6; }}
        .stats {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); gap: 15px; margin: 20px 0; }}
        .stat-item {{ background-color: #fff; padding: 15px; border-radius: 8px; text-align: center; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }}
        .stat-label {{ color: #8590a6; font-size: 12px; margin-bottom: 5px; }}
        .stat-value {{ color: #00a1d6; font-size: 18px; font-weight: bold; }}
        .tags {{ display: flex; flex-wrap: wrap; gap: 8px; margin: 15px 0; }}
        .tag {{ background-color: #e5f2ff; color: #00a1d6; padding: 4px 12px; border-radius: 15px; font-size: 12px; }}
        .description {{ background-color: #fff; padding: 20px; border-radius: 8px; margin: 20px 0; white-space: pre-wrap; word-wrap: break-word; }}
        .summary {{ background-color: #fff; padding: 20px; border-radius: 8px; margin: 20px 0; border-left: 4px solid #ff9500; }}
        .footer {{ margin-top: 40px; padding-top: 20px; border-top: 1px solid #e5e5e5; color: #8590a6; font-size: 12px; text-align: center; }}
        a {{ color: #00a1d6; text-decoration: none; }}
        a:hover {{ text-decoration: underline; }}
    </style>
</head>
<body>
""")
        
        # 写入标题
        f.write(f'    <h1>{data["title"]}</h1>\n')
        
        # 写入元信息
        f.write(f'    <div class="meta">\n')
        f.write(f'        <p><strong>UP主</strong>: {data["author"]}</p>\n')
        f.write(f'        <p><strong>发布时间</strong>: {data["metadata"]["publish_time"]}</p>\n')
        f.write(f'        <p><strong>视频分区</strong>: {data["metadata"]["category"]}</p>\n')
        if data['metadata']['duration']:
            f.write(f'        <p><strong>视频时长</strong>: {data["metadata"]["duration"]}</p>\n')
        f.write(f'        <p><strong>抓取时间</strong>: {data["fetch_time"]}</p>\n')
        f.write(f'        <p><strong>原始链接</strong>: <a href="{data["url"]}" target="_blank">{data["url"]}</a></p>\n')
        f.write(f'    </div>\n')
        
        # 写入统计数据
        f.write(f'    <h2>统计数据</h2>\n')
        f.write(f'    <div class="stats">\n')
        stats = data['stats']
        stat_items = [
            ("播放量", stats['play_count']),
            ("弹幕数", stats['danmaku_count']),
            ("点赞数", stats['like_count']),
            ("投币数", stats['coin_count']),
            ("收藏数", stats['favorite_count'])
        ]
        
        for label, value in stat_items:
            if value:
                f.write(f'        <div class="stat-item">\n')
                f.write(f'            <div class="stat-label">{label}</div>\n')
                f.write(f'            <div class="stat-value">{value}</div>\n')
                f.write(f'        </div>\n')
        f.write(f'    </div>\n')
        
        # 写入视频标签
        if data['metadata']['tags']:
            f.write(f'    <h2>视频标签</h2>\n')
            f.write(f'    <div class="tags">\n')
            for tag in data['metadata']['tags']:
                f.write(f'        <span class="tag">{tag}</span>\n')
            f.write(f'    </div>\n')
        
        # 写入视频描述
        if data['description']:
            f.write(f'    <h2>视频描述</h2>\n')
            f.write(f'    <div class="description">{data["description"]}</div>\n')
        
        # 写入内容总结
        f.write(f'    <h2>内容分析总结</h2>\n')
        f.write(f'    <div class="summary">\n')
        # 将总结文本转换为HTML段落
        summary_lines = summary.split('\n')
        for line in summary_lines:
            if line.strip():
                if line.startswith('==='):
                    f.write(f'        <h3>{line.strip("= ")}</h3>\n')
                elif '：' in line:
                    f.write(f'        <p><strong>{line.split("：")[0]}：</strong>{line.split("：")[1]}</p>\n')
                else:
                    f.write(f'        <p>{line}</p>\n')
        f.write(f'    </div>\n')
        
        # 页脚
        f.write("""    <div class="footer">
        本文件由B站视频内容提取工具自动生成
    </div>
</body>
</html>""")
    
        print(f"已保存到HTML文件: {output_path}")


def main():
    """主函数"""
    # 目标URL - 用户提供的B站视频链接
    url = "https://www.bilibili.com/video/BV1ELYdzxE6h/?spm_id_from=333.1391.0.0&vd_source=496e27d1f2f4d112f58c45a691290275"
    
    print("开始爬取B站视频信息...")
    print(f"目标视频: {url}")
    print("-" * 50)
    
    # 获取内容
    data = fetch_bilibili_video_info(url)
    
    if data:
        print("\n" + "=" * 50)
        print("视频信息爬取成功！")
        print("=" * 50)
        
        # 生成并显示总结
        summary = generate_summary(data)
        print("\n视频内容总结：")
        print("-" * 50)
        print(summary)
        print("-" * 50)
        
        # 保存为Markdown
        markdown_path = "D:\\VS_Code\\bilibili_video_summary.md"
        save_to_markdown(data, markdown_path)
        
        # 保存为HTML
        html_path = "D:\\VS_Code\\bilibili_video_summary.html"
        save_to_html(data, html_path)
        
        # 保存为JSON
        json_path = "D:\\VS_Code\\bilibili_video_data.json"
        with open(json_path, 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
        print(f"已保存到JSON文件: {json_path}")
        
        print("\n提取完成！")
        print(f"视频标题: {data['title']}")
        print(f"UP主: {data['author']}")
        print(f"播放量: {data['stats']['play_count']}")
        print(f"点赞数: {data['stats']['like_count']}")
        print(f"文件已保存到:")
        print(f"  - Markdown: {markdown_path}")
        print(f"  - HTML: {html_path}")
        print(f"  - JSON: {json_path}")
    else:
        print("\n未能提取视频信息，请检查网络连接或URL是否正确")


if __name__ == "__main__":
    main()
