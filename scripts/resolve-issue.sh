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
# Supported patterns:
#   - "update readme"    -> Updates README.md
#   - "update footer"    -> Updates footer text in App.jsx
#   - "change title"     -> Updates page title in App.jsx
#   - "add feature"      -> Adds placeholder feature
#   - "fix bug"          -> Applies bug fix patterns
#   - "update style"     -> Modifies CSS styles
#
# To extend: Add new pattern handlers in the main case block below.
# To integrate AI: Replace pattern matching with API call in the
#                  ai_resolve() function.
# =============================================================================

set -euo pipefail

echo "=========================================="
echo "Auto-resolve Issue #${ISSUE_NUMBER}"
echo "Title: ${ISSUE_TITLE}"
echo "=========================================="

# Convert title to lowercase for matching
TITLE_LOWER=$(echo "$ISSUE_TITLE" | tr '[:upper:]' '[:lower:]')
BODY_LOWER=$(echo "$ISSUE_BODY" | tr '[:upper:]' '[:lower:]')

CHANGED=false

# ---------------------------------------------------------------------------
# Pattern: Update README
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change|edit).*(readme|documentation|docs)"; then
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
# Pattern: Update footer text
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change).*(footer|bottom)"; then
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
# Pattern: Change page title / heading
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change).*(title|heading|header)"; then
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
# Pattern: Update styles / colors
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(update|modify|change).*(style|color|theme|css)"; then
    echo "[MATCH] Updating styles..."

    FRONTEND_CSS="ex01/frontend/src/App.css"
    if [ -f "$FRONTEND_CSS" ]; then
        # If body contains a hex color, update the gradient
        HEX_COLOR=$(echo "$ISSUE_BODY" | grep -oE '#[0-9a-fA-F]{6}' | head -1)
        if [ -n "$HEX_COLOR" ]; then
            sed -i "s/#667eea/${HEX_COLOR}/g" "$FRONTEND_CSS"
            CHANGED=true
        fi
    fi
fi

# ---------------------------------------------------------------------------
# Pattern: Add new feature / component
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(add|create|new).*(feature|function|component)"; then
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
# Pattern: Fix bug
# ---------------------------------------------------------------------------
if echo "$TITLE_LOWER" | grep -qE "(fix|bug|error|broken|issue)"; then
    echo "[MATCH] Attempting bug fix..."

    # Check if the issue body mentions specific files
    MENTIONED_FILE=$(echo "$ISSUE_BODY" | grep -oE '[a-zA-Z0-9_/]+\.(jsx?|tsx?|css|java|py)' | head -1)
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
