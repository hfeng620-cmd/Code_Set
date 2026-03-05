"""
知乎页面爬虫 - 提取问题和第一个回答
"""
from DrissionPage import ChromiumPage
import time
import json
from datetime import datetime
import re

def fetch_zhihu_content(url: str) -> dict:
    """
    获取知乎页面内容并提取问题和第一个回答
    
    Args:
        url: 知乎问题回答链接
        
    Returns:
        dict: 包含提取内容的字典
    """
    print(f"正在访问知乎页面: {url}")
    
    # 创建浏览器页面
    page = ChromiumPage()
    
    try:
        # 访问页面
        page.get(url)
        time.sleep(5)  # 等待页面加载
        
        print(f"页面标题: {page.title}")
        
        # 获取页面HTML
        html = page.html
        
        # 保存原始HTML以便调试
        with open('zhihu_raw.html', 'w', encoding='utf-8') as f:
            f.write(html)
        print("已保存原始页面到 zhihu_raw.html")
        
        # 提取问题标题
        question_title = ""
        # 尝试多种选择器
        selectors = [
            'h1.QuestionHeader-title',
            '.QuestionHeader .QuestionHeader-title',
            'h1[data-za-detail-view-element_name="Title"]',
            '.QuestionHeader-main h1',
            'h1'
        ]
        
        for selector in selectors:
            elements = page.eles(selector)
            if elements:
                question_title = elements[0].text
                print(f"找到问题标题: {question_title}")
                break
        
        # 提取问题描述
        question_description = ""
        desc_selectors = [
            '.QuestionRichText .RichText',
            '.QuestionHeader-detail .RichText',
            '.QuestionHeader-detail',
            '.QuestionHeader .RichText'
        ]
        
        for selector in desc_selectors:
            elements = page.eles(selector)
            if elements:
                question_description = elements[0].text
                print(f"找到问题描述: {question_description[:100]}...")
                break
        
        # 提取第一个回答（跑酱er的回答）
        first_answer = ""
        answer_author = ""
        
        # 首先尝试找到所有回答
        answer_selectors = [
            '.List .List-item',  # 回答列表项
            '.AnswerItem',  # 回答项
            '.ContentItem.AnswerItem'  # 内容项-回答项
        ]
        
        for selector in answer_selectors:
            answers = page.eles(selector)
            if answers:
                print(f"找到 {len(answers)} 个回答")
                
                # 查找作者为"跑酱er"的回答
                for answer in answers:
                    # 尝试获取作者信息
                    author_selectors = [
                        '.AuthorInfo .AuthorInfo-name',
                        '.AuthorInfo-name',
                        '.UserLink.AuthorInfo-name',
                        '.AuthorInfo .UserLink'
                    ]
                    
                    for auth_selector in author_selectors:
                        author_elements = answer.eles(auth_selector)
                        if author_elements:
                            current_author = author_elements[0].text
                            if "跑酱er" in current_author:
                                answer_author = current_author
                                print(f"找到目标作者: {answer_author}")
                                
                                # 提取回答内容
                                content_selectors = [
                                    '.RichContent .RichText',
                                    '.RichContent-inner',
                                    '.AnswerItem .RichText',
                                    '.ContentItem .RichText'
                                ]
                                
                                for content_selector in content_selectors:
                                    content_elements = answer.eles(content_selector)
                                    if content_elements:
                                        first_answer = content_elements[0].text
                                        print(f"找到回答内容，长度: {len(first_answer)} 字符")
                                        break
                                break
                    if first_answer:
                        break
                break
        
        # 如果没找到特定作者，取第一个回答
        if not first_answer and answers:
            print("未找到'跑酱er'的回答，取第一个回答")
            first_answer_item = answers[0]
            
            # 获取作者
            author_elements = first_answer_item.eles('.AuthorInfo .AuthorInfo-name')
            if author_elements:
                answer_author = author_elements[0].text
                print(f"第一个回答作者: {answer_author}")
            
            # 获取内容
            content_elements = first_answer_item.eles('.RichContent .RichText')
            if content_elements:
                first_answer = content_elements[0].text
                print(f"第一个回答内容，长度: {len(first_answer)} 字符")
        
        # 提取回答时间
        answer_time = ""
        time_selectors = [
            '.ContentItem-time',
            '.AnswerItem .ContentItem-time',
            '.ContentItem .ContentItem-time'
        ]
        
        for selector in time_selectors:
            time_elements = page.eles(selector)
            if time_elements:
                answer_time = time_elements[0].text
                print(f"回答时间: {answer_time}")
                break
        
        # 提取点赞数
        upvote_count = ""
        upvote_selectors = [
            '.VoteButton--up',
            '.AnswerItem .VoteButton',
            '.ContentItem-actions .VoteButton'
        ]
        
        for selector in upvote_selectors:
            upvote_elements = page.eles(selector)
            if upvote_elements:
                upvote_count = upvote_elements[0].text
                print(f"点赞数: {upvote_count}")
                break
        
        # 构建结果
        result = {
            "url": url,
            "title": question_title,
            "description": question_description,
            "answer": {
                "author": answer_author,
                "content": first_answer,
                "time": answer_time,
                "upvotes": upvote_count
            },
            "fetch_time": datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        }
        
        return result
        
    except Exception as e:
        print(f"爬取过程中发生错误: {e}")
        return None
        
    finally:
        # 关闭浏览器
        page.quit()

def save_to_markdown(data: dict, output_path: str):
    """
    将数据保存为Markdown格式
    
    Args:
        data: 提取的数据
        output_path: 输出文件路径
    """
    if not data:
        print("没有数据可保存")
        return
    
    with open(output_path, 'w', encoding='utf-8') as f:
        # 写入标题
        f.write(f"# {data['title']}\n\n")
        
        # 写入元信息
        f.write(f"**来源**: [知乎链接]({data['url']})\n\n")
        f.write(f"**抓取时间**: {data['fetch_time']}\n\n")
        
        # 写入问题描述
        if data['description']:
            f.write("## 问题描述\n\n")
            f.write(f"{data['description']}\n\n")
        
        # 写入回答
        f.write("## 回答\n\n")
        f.write(f"**作者**: {data['answer']['author']}\n\n")
        
        if data['answer']['time']:
            f.write(f"**回答时间**: {data['answer']['time']}\n\n")
        
        if data['answer']['upvotes']:
            f.write(f"**点赞数**: {data['answer']['upvotes']}\n\n")
        
        f.write("### 回答内容\n\n")
        f.write(f"{data['answer']['content']}\n\n")
        
        # 添加分隔线
        f.write("---\n\n")
        f.write("*本文件由知乎内容提取工具自动生成*\n")
    
    print(f"已保存到Markdown文件: {output_path}")

def save_to_html(data: dict, output_path: str):
    """
    将数据保存为HTML格式
    
    Args:
        data: 提取的数据
        output_path: 输出文件路径
    """
    if not data:
        print("没有数据可保存")
        return
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write("""<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>知乎内容提取 - {title}</title>
    <style>
        body {{ font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'PingFang SC', 'Hiragino Sans GB', 'Microsoft YaHei', 'Helvetica Neue', Helvetica, Arial, sans-serif; line-height: 1.6; color: #1a1a1a; max-width: 800px; margin: 0 auto; padding: 20px; }}
        h1 {{ color: #0084ff; border-bottom: 1px solid #ebebeb; padding-bottom: 10px; }}
        h2 {{ color: #262626; margin-top: 30px; }}
        h3 {{ color: #595959; }}
        .meta {{ color: #8590a6; font-size: 14px; margin-bottom: 20px; }}
        .answer {{ background-color: #f6f6f6; padding: 20px; border-radius: 8px; margin: 20px 0; }}
        .author {{ color: #175199; font-weight: bold; }}
        .content {{ white-space: pre-wrap; word-wrap: break-word; }}
        .footer {{ margin-top: 40px; padding-top: 20px; border-top: 1px solid #ebebeb; color: #8590a6; font-size: 12px; text-align: center; }}
    </style>
</head>
<body>
""".format(title=data['title']))
        
        # 写入标题
        f.write(f"    <h1>{data['title']}</h1>\n")
        
        # 写入元信息
        f.write(f'    <div class="meta">\n')
        f.write(f'        <p><strong>来源</strong>: <a href="{data["url"]}" target="_blank">知乎链接</a></p>\n')
        f.write(f'        <p><strong>抓取时间</strong>: {data["fetch_time"]}</p>\n')
        f.write(f'    </div>\n')
        
        # 写入问题描述
        if data['description']:
            f.write(f'    <h2>问题描述</h2>\n')
            f.write(f'    <div class="content">{data["description"]}</div>\n')
        
        # 写入回答
        f.write(f'    <h2>回答</h2>\n')
        f.write(f'    <div class="answer">\n')
        f.write(f'        <div class="author">{data["answer"]["author"]}</div>\n')
        
        if data['answer']['time']:
            f.write(f'        <div class="meta">回答时间: {data["answer"]["time"]}</div>\n')
        
        if data['answer']['upvotes']:
            f.write(f'        <div class="meta">点赞数: {data["answer"]["upvotes"]}</div>\n')
        
        f.write(f'        <h3>回答内容</h3>\n')
        f.write(f'        <div class="content">{data["answer"]["content"]}</div>\n')
        f.write(f'    </div>\n')
        
        # 页脚
        f.write("""    <div class="footer">
        本文件由知乎内容提取工具自动生成
    </div>
</body>
</html>""")
    
    print(f"已保存到HTML文件: {output_path}")

def main():
    """主函数"""
    # 目标URL
    url = "https://www.zhihu.com/question/412311476/answer/1995150376387748664?utm_psn=1998670171783569760"
    
    # 获取内容
    data = fetch_zhihu_content(url)
    
    if data:
        # 保存为Markdown
        save_to_markdown(data, "D:\\VS_Code\\zhihu_content.md")
        
        # 保存为HTML
        save_to_html(data, "D:\\VS_Code\\zhihu_content.html")
        
        # 也保存为JSON
        with open("D:\\VS_Code\\zhihu_content.json", 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
        print("已保存到JSON文件: D:\\VS_Code\\zhihu_content.json")
        
        print("\n提取完成！")
        print(f"问题标题: {data['title']}")
        print(f"回答作者: {data['answer']['author']}")
        print(f"回答内容长度: {len(data['answer']['content'])} 字符")
    else:
        print("未能提取内容")

if __name__ == "__main__":
    main()