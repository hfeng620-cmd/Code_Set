"""
知乎内容提取 - 使用Selenium
"""
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.chrome.options import Options
import time
import json
from datetime import datetime
import os

def setup_driver():
    """设置Chrome驱动"""
    chrome_options = Options()
    chrome_options.add_argument('--headless')  # 无头模式
    chrome_options.add_argument('--no-sandbox')
    chrome_options.add_argument('--disable-dev-shm-usage')
    chrome_options.add_argument('--disable-gpu')
    chrome_options.add_argument('--window-size=1920,1080')
    chrome_options.add_argument('--user-agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36')
    
    driver = webdriver.Chrome(options=chrome_options)
    return driver

def fetch_zhihu_content(url: str) -> dict:
    """
    使用Selenium获取知乎页面内容
    
    Args:
        url: 知乎问题回答链接
        
    Returns:
        dict: 包含提取内容的字典
    """
    driver = None
    try:
        print(f"正在使用Selenium访问知乎页面: {url}")
        driver = setup_driver()
        driver.get(url)
        
        # 等待页面加载
        wait = WebDriverWait(driver, 20)
        
        # 检查是否在登录页面
        if "登录" in driver.title or "signin" in driver.current_url:
            print("检测到登录页面，尝试等待页面重定向...")
            time.sleep(5)
            
            # 保存截图以便调试
            driver.save_screenshot('zhihu_login_screenshot.png')
            print("已保存登录页面截图到 zhihu_login_screenshot.png")
            
            # 保存页面源码
            with open('zhihu_selenium_login.html', 'w', encoding='utf-8') as f:
                f.write(driver.page_source)
            print("已保存登录页面源码到 zhihu_selenium_login.html")
            
            # 尝试直接查找内容，即使有登录弹窗
            print("尝试绕过登录弹窗查找内容...")
        
        # 保存原始HTML
        with open('zhihu_selenium_raw.html', 'w', encoding='utf-8') as f:
            f.write(driver.page_source)
        print(f"已保存原始页面到 zhihu_selenium_raw.html，大小: {len(driver.page_source)} 字符")
        
        # 提取问题标题
        question_title = ""
        title_selectors = [
            'h1.QuestionHeader-title',
            'h1[data-za-detail-view-element_name="Title"]',
            '.QuestionHeader-main h1',
            'h1'
        ]
        
        for selector in title_selectors:
            try:
                element = wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, selector)))
                question_title = element.text.strip()
                print(f"找到问题标题: {question_title}")
                break
            except:
                continue
        
        # 提取问题描述
        question_description = ""
        desc_selectors = [
            '.QuestionRichText .RichText',
            '.QuestionHeader-detail .RichText',
            '.QuestionHeader-detail',
            '.QuestionHeader .RichText'
        ]
        
        for selector in desc_selectors:
            try:
                element = driver.find_element(By.CSS_SELECTOR, selector)
                question_description = element.text.strip()
                print(f"找到问题描述: {question_description[:100]}...")
                break
            except:
                continue
        
        # 提取回答
        answers = []
        answer_selectors = [
            '.List .List-item',
            '.AnswerItem',
            '.ContentItem.AnswerItem'
        ]
        
        for selector in answer_selectors:
            try:
                answers = driver.find_elements(By.CSS_SELECTOR, selector)
                if answers:
                    print(f"找到 {len(answers)} 个回答")
                    break
            except:
                continue
        
        target_author = ""
        target_content = ""
        
        # 查找作者为"跑酱er"的回答
        for answer in answers:
            try:
                # 查找作者信息
                author_selectors = [
                    '.AuthorInfo .AuthorInfo-name',
                    '.AuthorInfo-name',
                    '.UserLink.AuthorInfo-name'
                ]
                
                author_element = None
                for auth_selector in author_selectors:
                    try:
                        author_element = answer.find_element(By.CSS_SELECTOR, auth_selector)
                        break
                    except:
                        continue
                
                if author_element:
                    author = author_element.text.strip()
                    if "跑酱er" in author:
                        target_author = author
                        print(f"找到目标作者: {target_author}")
                        
                        # 提取回答内容
                        content_selectors = [
                            '.RichContent .RichText',
                            '.RichContent-inner',
                            '.AnswerItem .RichText'
                        ]
                        
                        for content_selector in content_selectors:
                            try:
                                content_element = answer.find_element(By.CSS_SELECTOR, content_selector)
                                target_content = content_element.text.strip()
                                print(f"找到回答内容，长度: {len(target_content)} 字符")
                                break
                            except:
                                continue
                        break
            except Exception as e:
                continue
        
        # 如果没找到特定作者，取第一个回答
        if not target_content and answers:
            print("未找到'跑酱er'的回答，取第一个回答")
            try:
                first_answer = answers[0]
                
                # 获取作者
                try:
                    author_element = first_answer.find_element(By.CSS_SELECTOR, '.AuthorInfo .AuthorInfo-name')
                    target_author = author_element.text.strip()
                    print(f"第一个回答作者: {target_author}")
                except:
                    pass
                
                # 获取内容
                try:
                    content_element = first_answer.find_element(By.CSS_SELECTOR, '.RichContent .RichText')
                    target_content = content_element.text.strip()
                    print(f"第一个回答内容，长度: {len(target_content)} 字符")
                except:
                    pass
            except Exception as e:
                print(f"提取第一个回答时出错: {e}")
        
        # 提取回答时间
        answer_time = ""
        time_selectors = [
            '.ContentItem-time',
            '.AnswerItem .ContentItem-time'
        ]
        
        for selector in time_selectors:
            try:
                time_element = driver.find_element(By.CSS_SELECTOR, selector)
                answer_time = time_element.text.strip()
                print(f"回答时间: {answer_time}")
                break
            except:
                continue
        
        # 提取点赞数
        upvote_count = ""
        upvote_selectors = [
            '.VoteButton--up',
            '.AnswerItem .VoteButton'
        ]
        
        for selector in upvote_selectors:
            try:
                upvote_element = driver.find_element(By.CSS_SELECTOR, selector)
                upvote_count = upvote_element.text.strip()
                print(f"点赞数: {upvote_count}")
                break
            except:
                continue
        
        # 构建结果
        result = {
            "url": url,
            "title": question_title,
            "description": question_description,
            "answer": {
                "author": target_author,
                "content": target_content,
                "time": answer_time,
                "upvotes": upvote_count
            },
            "fetch_time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            "source": "selenium"
        }
        
        return result
        
    except Exception as e:
        print(f"Selenium爬取过程中发生错误: {e}")
        import traceback
        traceback.print_exc()
        return None
        
    finally:
        if driver:
            driver.quit()

def save_to_markdown(data: dict, output_path: str):
    """保存为Markdown格式"""
    if not data:
        print("没有数据可保存")
        return
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(f"# {data['title']}\n\n")
        f.write(f"**来源**: [知乎链接]({data['url']})\n\n")
        f.write(f"**抓取时间**: {data['fetch_time']}\n\n")
        
        if data['description']:
            f.write("## 问题描述\n\n")
            f.write(f"{data['description']}\n\n")
        
        f.write("## 回答\n\n")
        f.write(f"**作者**: {data['answer']['author']}\n\n")
        
        if data['answer']['time']:
            f.write(f"**回答时间**: {data['answer']['time']}\n\n")
        
        if data['answer']['upvotes']:
            f.write(f"**点赞数**: {data['answer']['upvotes']}\n\n")
        
        f.write("### 回答内容\n\n")
        f.write(f"{data['answer']['content']}\n\n")
        
        f.write("---\n\n")
        f.write("*本文件由知乎内容提取工具自动生成*\n")
    
    print(f"已保存到Markdown文件: {output_path}")

def save_to_html(data: dict, output_path: str):
    """保存为HTML格式"""
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
        
        f.write(f"    <h1>{data['title']}</h1>\n")
        f.write(f'    <div class="meta">\n')
        f.write(f'        <p><strong>来源</strong>: <a href="{data["url"]}" target="_blank">知乎链接</a></p>\n')
        f.write(f'        <p><strong>抓取时间</strong>: {data["fetch_time"]}</p>\n')
        f.write(f'    </div>\n')
        
        if data['description']:
            f.write(f'    <h2>问题描述</h2>\n')
            f.write(f'    <div class="content">{data["description"]}</div>\n')
        
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
        
        f.write("""    <div class="footer">
        本文件由知乎内容提取工具自动生成
    </div>
</body>
</html>""")
    
    print(f"已保存到HTML文件: {output_path}")

def main():
    """主函数"""
    url = "https://www.zhihu.com/question/412311476/answer/1995150376387748664?utm_psn=1998670171783569760"
    
    print("开始使用Selenium提取知乎内容...")
    data = fetch_zhihu_content(url)
    
    if data:
        print("\n提取成功！")
        print(f"问题标题: {data['title']}")
        print(f"回答作者: {data['answer']['author']}")
        print(f"回答内容长度: {len(data['answer']['content'])} 字符")
        
        # 保存文件
        save_to_markdown(data, "D:\\VS_Code\\zhihu_content.md")
        save_to_html(data, "D:\\VS_Code\\zhihu_content.html")
        
        # 保存JSON
        with open("D:\\VS_Code\\zhihu_content.json", 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
        print("已保存到JSON文件: D:\\VS_Code\\zhihu_content.json")
        
        print("\n所有文件已保存到 D:\\VS_Code 目录")
    else:
        print("未能提取内容，请检查网络或页面结构")

if __name__ == "__main__":
    main()