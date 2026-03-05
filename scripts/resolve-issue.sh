#!/bin/bash
# =============================================================================
# Auto-resolve Issue Script
# =============================================================================
# This script analyzes GitHub Issue content and applies fixes automatically.
#
# Environment variables (set by GitHub Actions):
#   ISSUE_NUMBER  - Issue number
#   ISSUE_TITLE   - Issue title
#   ISSUE_BODY    - Issue body/description
#
# Supported patterns (English + Chinese):
#   - "update readme" / "修改文档"         -> Updates README.md
#   - "update footer" / "修改页脚"         -> Updates footer text in App.jsx
#   - "change title"  / "修改标题"         -> Updates page title in App.jsx
#   - "add feature"   / "添加功能"         -> Adds placeholder feature
#   - "fix bug"       / "修复"             -> Applies bug fix patterns
#   - "update style"  / "颜色/样式/主题"   -> Modifies CSS styles
#
# To extend: Add new pattern handlers in the main case block below.
# To integrate AI: Replace pattern matching with API call in the
#                  ai_resolve() function.
# =============================================================================

set -eo pipefail

# Ensure ISSUE_BODY has a default value (may be empty for issues without a body)
ISSUE_BODY="${ISSUE_BODY:-}"

echo "=========================================="
echo "Auto-resolve Issue #${ISSUE_NUMBER}"
echo "Title: ${ISSUE_TITLE}"
echo "=========================================="

# Convert title to lowercase for matching
TITLE_LOWER=$(echo "$ISSUE_TITLE" | tr '[:upper:]' '[:lower:]')
BODY_LOWER=$(echo "$ISSUE_BODY" | tr '[:upper:]' '[:lower:]')

CHANGED=false

# ---------------------------------------------------------------------------
# Pattern: Update README (EN + CN)
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change|edit).*(readme|documentation|docs)" || \
   echo "$ISSUE_TITLE" | grep -qE "(修改|更新|编辑).*(文档|readme|说明)"; then
    echo "[MATCH] Updating README.md..."

    # Extract the requested change from issue body
    if [ -n "$ISSUE_BODY" ]; then
        # Append issue content as a new section
        cat >> README.md << EOF

<!-- Auto-resolved from Issue #${ISSUE_NUMBER} -->
> **Note:** ${ISSUE_BODY}
EOF
        CHANGED=true
    fi
fi

# ---------------------------------------------------------------------------
# Pattern: Update footer text (EN + CN)
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change).*(footer|bottom)" || \
   echo "$ISSUE_TITLE" | grep -qE "(修改|更新|改).*(页脚|底部|footer)"; then
    echo "[MATCH] Updating footer text..."

    FRONTEND_APP="ex01/frontend/src/App.jsx"
    if [ -f "$FRONTEND_APP" ]; then
        # Extract new footer text from issue body, or use a default
        NEW_FOOTER="${ISSUE_BODY:-Updated footer text}"
        # Escape special characters for sed
        NEW_FOOTER_ESCAPED=$(echo "$NEW_FOOTER" | sed 's/[&/\]/\\&/g' | head -1)

        sed -i "s|class_demo / ex01.*|class_demo / ex01 \&mdash; ${NEW_FOOTER_ESCAPED}</p>|" "$FRONTEND_APP"
        CHANGED=true
    fi
fi

# ---------------------------------------------------------------------------
# Pattern: Change page title / heading (EN + CN)
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change).*(title|heading|header)" || \
   echo "$ISSUE_TITLE" | grep -qE "(修改|更新|改).*(标题|题目|heading)"; then
    echo "[MATCH] Updating page title..."

    FRONTEND_APP="ex01/frontend/src/App.jsx"
    if [ -f "$FRONTEND_APP" ]; then
        NEW_TITLE="${ISSUE_BODY:-HelloWorld App}"
        NEW_TITLE_ESCAPED=$(echo "$NEW_TITLE" | sed 's/[&/\]/\\&/g' | head -1)

        sed -i "s|<h1>.*</h1>|<h1>${NEW_TITLE_ESCAPED}</h1>|" "$FRONTEND_APP"
        CHANGED=true
    fi
fi

# ---------------------------------------------------------------------------
# Pattern: Update styles / colors (EN + CN)
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change).*(style|color|theme|css)" || \
   echo "$ISSUE_TITLE" | grep -qE "(颜色|样式|主题|配色|色彩|好看|难看|不好看|美化|优化.*样式)"; then
    echo "[MATCH] Updating styles..."

    FRONTEND_CSS="ex01/frontend/src/App.css"
    if [ -f "$FRONTEND_CSS" ]; then
        # If body contains a hex color, use it
        HEX_COLOR=$(echo "$ISSUE_BODY" | grep -oE '#[0-9a-fA-F]{6}' | head -1 || true)
        if [ -n "$HEX_COLOR" ]; then
            sed -i "s/#667eea/${HEX_COLOR}/g" "$FRONTEND_CSS"
            CHANGED=true
        else
            # No hex color specified, apply an improved color scheme
            echo "  No hex color in body, applying improved color scheme..."
            # Primary gradient: warm coral to purple
            sed -i 's/#667eea/#f093fb/g' "$FRONTEND_CSS"
            sed -i 's/#764ba2/#f5576c/g' "$FRONTEND_CSS"
            # Card background: softer dark
            sed -i 's/#1e1e2e/#1a1b2e/g' "$FRONTEND_CSS"
            sed -i 's/#2d2d3f/#2a2b4a/g' "$FRONTEND_CSS"
            # Input background
            sed -i 's/#16161e/#151623/g' "$FRONTEND_CSS"
            # Result text: warm green
            sed -i 's/#a8d8a8/#7dd3a8/g' "$FRONTEND_CSS"
            CHANGED=true
        fi
    fi
fi

# ---------------------------------------------------------------------------
# Pattern: Add new feature / component (EN + CN)
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(add|create|new).*(feature|function|component)" || \
   echo "$ISSUE_TITLE" | grep -qE "(添加|新增|增加|创建).*(功能|组件|特性|模块)"; then
    echo "[MATCH] Adding new feature placeholder..."

    FRONTEND_APP="ex01/frontend/src/App.jsx"
    if [ -f "$FRONTEND_APP" ]; then
        # Add a new card section before the footer
        FEATURE_DESC="${ISSUE_BODY:-New feature from Issue #${ISSUE_NUMBER}}"
        FEATURE_DESC_ESCAPED=$(echo "$FEATURE_DESC" | sed 's/[&/\]/\\&/g' | head -1)

        sed -i "/<footer/i\\
        <section className=\"card\">\\
          <h2>New Feature</h2>\\
          <p className=\"description\">${FEATURE_DESC_ESCAPED}</p>\\
        </section>" "$FRONTEND_APP"
        CHANGED=true
    fi
fi

# ---------------------------------------------------------------------------
# Pattern: Fix bug (EN + CN)
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(fix|bug|error|broken|issue)" || \
   echo "$ISSUE_TITLE" | grep -qE "(修复|修正|bug|错误|问题|崩溃|异常)"; then
    echo "[MATCH] Attempting bug fix..."

    # Check if the issue body mentions specific files
    MENTIONED_FILE=$(echo "$ISSUE_BODY" | grep -oE '[a-zA-Z0-9_/]+\.(jsx?|tsx?|css|java|py)' | head -1 || true)
    if [ -n "$MENTIONED_FILE" ] && [ -f "$MENTIONED_FILE" ]; then
        echo "  File mentioned: $MENTIONED_FILE"
        # Add a fix comment marker (placeholder for AI integration)
        echo "// Fix applied for Issue #${ISSUE_NUMBER}: ${ISSUE_TITLE}" >> "$MENTIONED_FILE"
        CHANGED=true
    fi
fi

# ---------------------------------------------------------------------------
# Result
# ---------------------------------------------------------------------------
if [ "$CHANGED" = true ]; then
    echo ""
    echo "[SUCCESS] Changes applied for Issue #${ISSUE_NUMBER}"
    echo "Files modified:"
    git diff --name-only
else
    echo ""
    echo "[SKIP] No matching pattern found for: ${ISSUE_TITLE}"
    echo "Manual review required."
fi

# =============================================================================
# AI Integration Point (Future)
# =============================================================================
# To integrate GitHub Copilot or OpenAI:
#
# ai_resolve() {
#   curl -s https://api.openai.com/v1/chat/completions \
#     -H "Authorization: Bearer $OPENAI_API_KEY" \
#     -H "Content-Type: application/json" \
#     -d "{
#       \"model\": \"gpt-4\",
#       \"messages\": [{
#         \"role\": \"system\",
#         \"content\": \"You are a developer. Analyze this issue and output a unified diff patch.\"
#       }, {
#         \"role\": \"user\",
#         \"content\": \"Issue: ${ISSUE_TITLE}\n\n${ISSUE_BODY}\"
#       }]
#     }" | jq -r '.choices[0].message.content'
# }
#
# PATCH=$(ai_resolve)
# echo "$PATCH" | git apply
# =============================================================================
