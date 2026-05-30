const crypto = require("crypto");

const STOP_WORDS = new Set([
  "the", "and", "for", "that", "this", "with", "you", "your", "are", "was", "were",
  "就是", "这个", "那个", "然后", "因为", "所以", "如果", "不是", "没有", "还是", "但是",
  "可以", "一下", "一个", "我们", "你们", "他们", "自己", "什么", "怎么", "这样", "现在",
  "感觉", "觉得", "可能", "应该", "真的", "哈哈", "哈哈哈"
]);

const FILLERS = [
  "哈哈", "哈哈哈", "笑死", "救命", "确实", "真的", "对对", "也是", "可以", "行吧",
  "离谱", "绝了", "懂了", "收到", "没事", "问题不大", "我感觉", "我觉得", "你知道吧"
];

function normalizeText(text) {
  return String(text || "")
    .replace(/^\uFEFF/, "")
    .replace(/\r\n/g, "\n")
    .replace(/\r/g, "\n")
    .trim();
}

function sanitizeText(text) {
  return normalizeText(text)
    .replace(/[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}/gi, "[邮箱]")
    .replace(/(?<!\d)1[3-9]\d{9}(?!\d)/g, "[手机号]")
    .replace(/\b\d{15}(\d{2}[0-9Xx])?\b/g, "[身份证]")
    .replace(/\b(?:\d{4}[- ]?){3}\d{4}\b/g, "[银行卡]")
    .replace(/\b(?:https?:\/\/|www\.)\S+/gi, "[链接]")
    .replace(/\b\d{6,12}\b/g, "[数字ID]");
}

function parseChatText(text) {
  const normalized = normalizeText(text);
  if (!normalized) return [];

  const lines = normalized.split("\n");
  const messages = [];
  let current = null;

  for (const rawLine of lines) {
    const line = rawLine.trim();
    if (!line) continue;

    const parsed = parseLine(line);
    if (parsed) {
      if (current) messages.push(current);
      current = parsed;
      continue;
    }

    if (current) {
      current.content = `${current.content}\n${line}`.trim();
    } else {
      current = {
        time: "",
        speaker: "unknown",
        content: line,
        source: "plain"
      };
    }
  }

  if (current) messages.push(current);

  return messages
    .map((message, index) => ({
      id: index + 1,
      time: message.time || "",
      speaker: cleanSpeaker(message.speaker || "unknown"),
      content: sanitizeText(message.content || ""),
      rawContentLength: String(message.content || "").length,
      source: message.source || "unknown"
    }))
    .filter((message) => message.content && !isSystemLine(message.content));
}

function parseLine(line) {
  const patterns = [
    /^(\d{4}[-/.]\d{1,2}[-/.]\d{1,2}\s+\d{1,2}:\d{2}(?::\d{2})?)\s+([^:：]{1,40})[:：]\s*(.*)$/,
    /^(\d{1,2}[-/.]\d{1,2}\s+\d{1,2}:\d{2}(?::\d{2})?)\s+([^:：]{1,40})[:：]\s*(.*)$/,
    /^(\d{1,2}:\d{2}(?::\d{2})?)\s+([^:：]{1,40})[:：]\s*(.*)$/,
    /^\[?(\d{4}[-/.]\d{1,2}[-/.]\d{1,2}\s+\d{1,2}:\d{2}(?::\d{2})?)\]?\s+\[?([^\]]{1,40})\]?\s*(.*)$/,
    /^([^:：]{1,40})[:：]\s*(.+)$/
  ];

  for (const pattern of patterns) {
    const match = line.match(pattern);
    if (!match) continue;

    if (match.length === 4) {
      return {
        time: match[1].trim(),
        speaker: match[2].trim(),
        content: match[3].trim(),
        source: "structured"
      };
    }

    return {
      time: "",
      speaker: match[1].trim(),
      content: match[2].trim(),
      source: "speaker-prefix"
    };
  }

  return null;
}

function cleanSpeaker(speaker) {
  return String(speaker || "unknown")
    .replace(/^微信名[:：]?/i, "")
    .replace(/\s+/g, " ")
    .trim()
    .slice(0, 40) || "unknown";
}

function isSystemLine(content) {
  return /^(以下为|消息记录|撤回了一条消息|已开启|以上是|微信红包|转账|语音通话|视频通话)/.test(content);
}

function distillPersona(input) {
  const rawText = input.rawText || "";
  const messages = parseChatText(rawText);
  const displayName = String(input.displayName || "未命名人格").trim().slice(0, 60) || "未命名人格";
  const consent = normalizeConsent(input.consent || {});
  const ownerEmail = String(input.ownerEmail || "").trim();
  const notes = String(input.notes || "").trim();
  const speakerStats = buildSpeakerStats(messages);
  const primarySpeaker = inferPrimarySpeaker(messages, speakerStats, displayName);
  const ownMessages = messages.filter((message) => message.speaker === primarySpeaker || primarySpeaker === "unknown");
  const samplePool = ownMessages.length >= 8 ? ownMessages : messages;
  const textForStats = samplePool.map((message) => message.content).join("\n");
  const sentenceStats = buildSentenceStats(samplePool);
  const words = topKeywords(textForStats, 24);
  const catchphrases = findCatchphrases(textForStats);
  const examples = sampleMessages(samplePool, 18);
  const topics = inferTopics(words, textForStats);
  const tone = inferTone(sentenceStats, catchphrases, textForStats);
  const boundaries = buildBoundaries(consent);
  const persona = {
    id: makeId(displayName, rawText),
    displayName,
    ownerEmail,
    notes,
    createdAt: new Date().toISOString(),
    version: 1,
    consent,
    source: {
      messageCount: messages.length,
      speakerCount: Object.keys(speakerStats).length,
      primarySpeaker,
      sanitized: true
    },
    style: {
      tone,
      averageLength: sentenceStats.averageLength,
      emojiRatio: sentenceStats.emojiRatio,
      questionRatio: sentenceStats.questionRatio,
      exclamationRatio: sentenceStats.exclamationRatio,
      catchphrases,
      keywords: words,
      topics
    },
    examples,
    memories: buildMemories(samplePool, topics),
    boundaries,
    systemPrompt: ""
  };

  persona.systemPrompt = buildSystemPrompt(persona);
  return persona;
}

function normalizeConsent(consent) {
  return {
    allowHumanReview: consent.allowHumanReview === true || consent.allowHumanReview === "true",
    allowTraining: consent.allowTraining === true || consent.allowTraining === "true",
    uploadedByOwner: consent.uploadedByOwner !== false && consent.uploadedByOwner !== "false",
    acceptedAt: consent.acceptedAt || new Date().toISOString(),
    policyVersion: consent.policyVersion || "local-mvp-v1"
  };
}

function buildSpeakerStats(messages) {
  const stats = {};
  for (const message of messages) {
    if (!stats[message.speaker]) {
      stats[message.speaker] = {
        count: 0,
        chars: 0
      };
    }
    stats[message.speaker].count += 1;
    stats[message.speaker].chars += message.content.length;
  }
  return stats;
}

function inferPrimarySpeaker(messages, stats, displayName) {
  if (!messages.length) return "unknown";
  const exact = Object.keys(stats).find((speaker) => speaker === displayName);
  if (exact) return exact;
  const fuzzy = Object.keys(stats).find((speaker) => displayName && (speaker.includes(displayName) || displayName.includes(speaker)));
  if (fuzzy) return fuzzy;
  return Object.entries(stats).sort((a, b) => {
    const countDiff = b[1].count - a[1].count;
    if (countDiff !== 0) return countDiff;
    return b[1].chars - a[1].chars;
  })[0][0];
}

function buildSentenceStats(messages) {
  if (!messages.length) {
    return {
      averageLength: 0,
      emojiRatio: 0,
      questionRatio: 0,
      exclamationRatio: 0
    };
  }

  const totalChars = messages.reduce((sum, message) => sum + message.content.length, 0);
  const emojiCount = messages.filter((message) => /[\u{1F300}-\u{1FAFF}]/u.test(message.content)).length;
  const questionCount = messages.filter((message) => /[?？]/.test(message.content)).length;
  const exclamationCount = messages.filter((message) => /[!！]/.test(message.content)).length;

  return {
    averageLength: Math.round(totalChars / messages.length),
    emojiRatio: round2(emojiCount / messages.length),
    questionRatio: round2(questionCount / messages.length),
    exclamationRatio: round2(exclamationCount / messages.length)
  };
}

function topKeywords(text, limit) {
  const cleaned = sanitizeText(text);
  const tokens = [];
  const english = cleaned.toLowerCase().match(/[a-z][a-z0-9_+-]{2,}/g) || [];
  tokens.push(...english);

  const chineseSegments = cleaned
    .replace(/[^\u4e00-\u9fa5]/g, " ")
    .split(/\s+/)
    .filter(Boolean);

  for (const segment of chineseSegments) {
    for (let size = 2; size <= 4; size += 1) {
      for (let index = 0; index <= segment.length - size; index += 1) {
        tokens.push(segment.slice(index, index + size));
      }
    }
  }

  const counts = new Map();
  for (const token of tokens) {
    if (STOP_WORDS.has(token) || /^\d+$/.test(token)) continue;
    counts.set(token, (counts.get(token) || 0) + 1);
  }

  return [...counts.entries()]
    .filter(([, count]) => count >= 2)
    .sort((a, b) => b[1] - a[1] || a[0].localeCompare(b[0], "zh-CN"))
    .slice(0, limit)
    .map(([word, count]) => ({ word, count }));
}

function findCatchphrases(text) {
  const result = [];
  for (const phrase of FILLERS) {
    const count = countOccurrences(text, phrase);
    if (count > 0) result.push({ phrase, count });
  }

  const repeated = [...text.matchAll(/([\u4e00-\u9fa5A-Za-z]{2,8})[，,。.!！?？\s]/g)]
    .map((match) => match[1])
    .filter((phrase) => !STOP_WORDS.has(phrase));
  const counts = new Map();
  for (const phrase of repeated) {
    counts.set(phrase, (counts.get(phrase) || 0) + 1);
  }

  for (const [phrase, count] of counts.entries()) {
    if (count >= 3 && result.every((item) => item.phrase !== phrase)) {
      result.push({ phrase, count });
    }
  }

  return result.sort((a, b) => b.count - a.count).slice(0, 10);
}

function countOccurrences(text, phrase) {
  if (!phrase) return 0;
  return String(text).split(phrase).length - 1;
}

function sampleMessages(messages, limit) {
  return messages
    .filter((message) => {
      const content = message.content.trim();
      return content.length >= 4 && content.length <= 180 && !/^\[[^\]]+\]$/.test(content);
    })
    .sort((a, b) => scoreExample(b.content) - scoreExample(a.content))
    .slice(0, limit)
    .map((message) => ({
      speaker: message.speaker,
      content: message.content,
      time: message.time
    }));
}

function scoreExample(content) {
  let score = Math.min(content.length, 80);
  if (/[?？!！]/.test(content)) score += 12;
  if (/[，。]/.test(content)) score += 8;
  if (FILLERS.some((phrase) => content.includes(phrase))) score += 10;
  return score;
}

function inferTopics(keywords, text) {
  const topicRules = [
    ["工作", ["工作", "项目", "需求", "老板", "客户", "会议", "代码", "上线"]],
    ["学习", ["学习", "考试", "课程", "论文", "学校", "老师", "资料"]],
    ["情绪", ["开心", "难受", "焦虑", "累", "烦", "压力", "喜欢", "讨厌"]],
    ["生活", ["吃饭", "睡觉", "回家", "出门", "天气", "买", "玩"]],
    ["关系", ["朋友", "家人", "对象", "聊天", "微信", "见面", "一起"]],
    ["兴趣", ["游戏", "电影", "音乐", "运动", "旅行", "拍照", "小说"]]
  ];
  const words = keywords.map((item) => item.word);
  const topics = [];
  for (const [name, hints] of topicRules) {
    const score = hints.reduce((sum, hint) => sum + countOccurrences(text, hint), 0)
      + words.filter((word) => hints.some((hint) => word.includes(hint) || hint.includes(word))).length;
    if (score > 0) topics.push({ name, score });
  }
  return topics.sort((a, b) => b.score - a.score).slice(0, 6);
}

function inferTone(stats, catchphrases, text) {
  const labels = [];
  if (stats.averageLength <= 12) labels.push("短句直接");
  if (stats.averageLength > 28) labels.push("表达细致");
  if (stats.questionRatio > 0.22) labels.push("常用反问和追问");
  if (stats.exclamationRatio > 0.16) labels.push("情绪表达明显");
  if (catchphrases.some((item) => /哈|笑死|救命|绝了/.test(item.phrase))) labels.push("轻松口语化");
  if (/谢谢|麻烦|辛苦|不好意思/.test(text)) labels.push("礼貌克制");
  if (/离谱|服了|无语|烦/.test(text)) labels.push("吐槽感较强");
  return labels.length ? labels : ["自然口语", "回应简洁"];
}

function buildMemories(messages, topics) {
  const memories = [];
  for (const topic of topics.slice(0, 4)) {
    memories.push({
      type: "topic",
      title: topic.name,
      content: `聊天记录里经常出现“${topic.name}”相关内容，回复时可以保持熟悉但不要编造事实。`
    });
  }

  const notable = messages
    .filter((message) => /我|自己|喜欢|讨厌|最近|以后|之前|一直/.test(message.content))
    .slice(0, 8)
    .map((message) => ({
      type: "sample",
      title: "可参考表达",
      content: message.content
    }));

  return memories.concat(notable).slice(0, 12);
}

function buildBoundaries(consent) {
  const boundaries = [
    "不要声称自己是真人本人，只能说明这是基于授权聊天记录生成的数字分身。",
    "遇到医疗、法律、金融、重大人生决定时，提醒对方咨询专业人士。",
    "不要输出身份证、手机号、地址、银行卡等敏感信息；原始记录中出现的隐私应保持脱敏。",
    "不知道的事实直接说明不确定，不要编造聊天记录中没有的信息。"
  ];

  if (!consent.allowHumanReview) {
    boundaries.push("用户未授权人工查看原文，后台运营人员只能查看脱敏摘要和人格包。");
  }

  if (!consent.allowTraining) {
    boundaries.push("用户未授权训练，本数据不得进入任何微调、蒸馏或共享训练集。");
  }

  return boundaries;
}

function buildSystemPrompt(persona) {
  const catchphrases = persona.style.catchphrases.map((item) => item.phrase).join("、") || "无明显固定口头禅";
  const keywords = persona.style.keywords.slice(0, 12).map((item) => item.word).join("、") || "无明显高频词";
  const examples = persona.examples
    .slice(0, 8)
    .map((item, index) => `${index + 1}. ${item.content}`)
    .join("\n");
  const memories = persona.memories
    .slice(0, 8)
    .map((item) => `- ${item.content}`)
    .join("\n");
  const boundaries = persona.boundaries.map((item) => `- ${item}`).join("\n");

  return [
    `你是“${persona.displayName}”的授权数字分身。`,
    "",
    "目标：用接近该用户的表达方式和对方聊天，但必须诚实说明自己是 AI 数字分身。",
    "",
    `语气特征：${persona.style.tone.join("、")}`,
    `平均句长：约 ${persona.style.averageLength} 字`,
    `常见口头禅：${catchphrases}`,
    `高频词：${keywords}`,
    "",
    "记忆线索：",
    memories || "- 暂无足够记忆线索。",
    "",
    "表达样例：",
    examples || "暂无足够样例。",
    "",
    "边界：",
    boundaries,
    "",
    "回复规则：",
    "- 先像朋友一样自然回应，不要写分析报告。",
    "- 默认短回复；需要解释时再展开。",
    "- 可以参考口头禅，但不要每句都硬塞。",
    "- 不确定的个人事实要说不确定。"
  ].join("\n");
}

function generateReply(persona, message) {
  const text = String(message || "").trim();
  const catchphrase = persona.style.catchphrases[0]?.phrase || "";
  const topic = persona.style.topics[0]?.name || "这件事";
  const tone = persona.style.tone.join("、") || "自然口语";
  const examples = persona.examples.map((item) => item.content);
  const similar = findSimilarExample(examples, text);

  if (!text) {
    return "你先说，我在。";
  }

  if (/你是谁|真的假的|真人|本人/.test(text)) {
    return `我是 ${persona.displayName} 的 AI 数字分身，基于授权聊天记录做了风格和记忆蒸馏，不是真人本人。`;
  }

  if (/删除|撤回|隐私|训练|授权/.test(text)) {
    return "这个要认真处理：可以在后台删除对应人格和数据；没有授权训练的数据不应该进入训练集。";
  }

  if (/[?？]$/.test(text)) {
    const prefix = catchphrase ? `${catchphrase}，` : "";
    return `${prefix}我倾向于先按“${topic}”这个方向看。按我这个说话习惯，应该会先确认具体情况，再给一个直接点的判断。`;
  }

  if (/难受|烦|焦虑|累|崩|压力/.test(text)) {
    return `${catchphrase ? `${catchphrase}，` : ""}先别急着硬扛。你可以把最卡的那一小段说清楚，我陪你一起拆一下。`;
  }

  if (similar) {
    return `我大概会像之前那种语气回：${similar}`;
  }

  return `${catchphrase ? `${catchphrase}，` : ""}我懂你的意思。按这个人格包现在学到的风格，会偏“${tone}”地回你：先把重点说清楚，再慢慢补细节。`;
}

function findSimilarExample(examples, message) {
  const chars = new Set([...message].filter((char) => /[\u4e00-\u9fa5A-Za-z0-9]/.test(char)));
  if (!chars.size) return "";
  let best = "";
  let bestScore = 0;
  for (const example of examples) {
    const score = [...chars].reduce((sum, char) => sum + (example.includes(char) ? 1 : 0), 0);
    if (score > bestScore) {
      bestScore = score;
      best = example;
    }
  }
  return bestScore >= 3 ? best : "";
}

function buildPersonaExportFiles(persona) {
  return [
    {
      name: "persona.json",
      content: JSON.stringify(persona, null, 2)
    },
    {
      name: "system_prompt.md",
      content: persona.systemPrompt
    },
    {
      name: "memory.md",
      content: persona.memories.map((item) => `- ${item.content}`).join("\n") || "暂无记忆。"
    },
    {
      name: "style_examples.jsonl",
      content: persona.examples.map((item) => JSON.stringify(item)).join("\n")
    },
    {
      name: "consent.json",
      content: JSON.stringify(persona.consent, null, 2)
    },
    {
      name: "README.md",
      content: [
        `# ${persona.displayName} 人格包`,
        "",
        "这是由 Talk with yourself MVP 生成的授权人格包。",
        "",
        "文件说明：",
        "- persona.json：完整人格结构化数据。",
        "- system_prompt.md：可放入云端模型或 Agent 的系统提示词。",
        "- memory.md：长期记忆线索。",
        "- style_examples.jsonl：可用于后续评测或训练的表达样例。",
        "- consent.json：上传和训练授权状态。",
        "",
        "注意：如果 consent.json 中 allowTraining=false，不要把该数据用于微调或共享训练集。"
      ].join("\n")
    }
  ];
}

function makeId(displayName, rawText) {
  const hash = crypto
    .createHash("sha1")
    .update(`${displayName}\n${rawText}\n${Date.now()}`)
    .digest("hex")
    .slice(0, 12);
  return `p_${hash}`;
}

function round2(number) {
  return Math.round(number * 100) / 100;
}

module.exports = {
  parseChatText,
  sanitizeText,
  distillPersona,
  buildSystemPrompt,
  generateReply,
  buildPersonaExportFiles
};
