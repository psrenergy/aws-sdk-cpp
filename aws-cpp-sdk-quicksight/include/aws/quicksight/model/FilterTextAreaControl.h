﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/quicksight/QuickSight_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/quicksight/model/TextAreaControlDisplayOptions.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace QuickSight
{
namespace Model
{

  /**
   * <p>A control to display a text box that is used to enter multiple
   * entries.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/quicksight-2018-04-01/FilterTextAreaControl">AWS
   * API Reference</a></p>
   */
  class AWS_QUICKSIGHT_API FilterTextAreaControl
  {
  public:
    FilterTextAreaControl();
    FilterTextAreaControl(Aws::Utils::Json::JsonView jsonValue);
    FilterTextAreaControl& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline const Aws::String& GetFilterControlId() const{ return m_filterControlId; }

    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline bool FilterControlIdHasBeenSet() const { return m_filterControlIdHasBeenSet; }

    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetFilterControlId(const Aws::String& value) { m_filterControlIdHasBeenSet = true; m_filterControlId = value; }

    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetFilterControlId(Aws::String&& value) { m_filterControlIdHasBeenSet = true; m_filterControlId = std::move(value); }

    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetFilterControlId(const char* value) { m_filterControlIdHasBeenSet = true; m_filterControlId.assign(value); }

    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithFilterControlId(const Aws::String& value) { SetFilterControlId(value); return *this;}

    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithFilterControlId(Aws::String&& value) { SetFilterControlId(std::move(value)); return *this;}

    /**
     * <p>The ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithFilterControlId(const char* value) { SetFilterControlId(value); return *this;}


    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline const Aws::String& GetTitle() const{ return m_title; }

    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline bool TitleHasBeenSet() const { return m_titleHasBeenSet; }

    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetTitle(const Aws::String& value) { m_titleHasBeenSet = true; m_title = value; }

    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetTitle(Aws::String&& value) { m_titleHasBeenSet = true; m_title = std::move(value); }

    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetTitle(const char* value) { m_titleHasBeenSet = true; m_title.assign(value); }

    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithTitle(const Aws::String& value) { SetTitle(value); return *this;}

    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithTitle(Aws::String&& value) { SetTitle(std::move(value)); return *this;}

    /**
     * <p>The title of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithTitle(const char* value) { SetTitle(value); return *this;}


    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline const Aws::String& GetSourceFilterId() const{ return m_sourceFilterId; }

    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline bool SourceFilterIdHasBeenSet() const { return m_sourceFilterIdHasBeenSet; }

    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetSourceFilterId(const Aws::String& value) { m_sourceFilterIdHasBeenSet = true; m_sourceFilterId = value; }

    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetSourceFilterId(Aws::String&& value) { m_sourceFilterIdHasBeenSet = true; m_sourceFilterId = std::move(value); }

    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline void SetSourceFilterId(const char* value) { m_sourceFilterIdHasBeenSet = true; m_sourceFilterId.assign(value); }

    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithSourceFilterId(const Aws::String& value) { SetSourceFilterId(value); return *this;}

    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithSourceFilterId(Aws::String&& value) { SetSourceFilterId(std::move(value)); return *this;}

    /**
     * <p>The source filter ID of the <code>FilterTextAreaControl</code>.</p>
     */
    inline FilterTextAreaControl& WithSourceFilterId(const char* value) { SetSourceFilterId(value); return *this;}


    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline const Aws::String& GetDelimiter() const{ return m_delimiter; }

    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline bool DelimiterHasBeenSet() const { return m_delimiterHasBeenSet; }

    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline void SetDelimiter(const Aws::String& value) { m_delimiterHasBeenSet = true; m_delimiter = value; }

    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline void SetDelimiter(Aws::String&& value) { m_delimiterHasBeenSet = true; m_delimiter = std::move(value); }

    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline void SetDelimiter(const char* value) { m_delimiterHasBeenSet = true; m_delimiter.assign(value); }

    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline FilterTextAreaControl& WithDelimiter(const Aws::String& value) { SetDelimiter(value); return *this;}

    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline FilterTextAreaControl& WithDelimiter(Aws::String&& value) { SetDelimiter(std::move(value)); return *this;}

    /**
     * <p>The delimiter that is used to separate the lines in text.</p>
     */
    inline FilterTextAreaControl& WithDelimiter(const char* value) { SetDelimiter(value); return *this;}


    /**
     * <p>The display options of a control.</p>
     */
    inline const TextAreaControlDisplayOptions& GetDisplayOptions() const{ return m_displayOptions; }

    /**
     * <p>The display options of a control.</p>
     */
    inline bool DisplayOptionsHasBeenSet() const { return m_displayOptionsHasBeenSet; }

    /**
     * <p>The display options of a control.</p>
     */
    inline void SetDisplayOptions(const TextAreaControlDisplayOptions& value) { m_displayOptionsHasBeenSet = true; m_displayOptions = value; }

    /**
     * <p>The display options of a control.</p>
     */
    inline void SetDisplayOptions(TextAreaControlDisplayOptions&& value) { m_displayOptionsHasBeenSet = true; m_displayOptions = std::move(value); }

    /**
     * <p>The display options of a control.</p>
     */
    inline FilterTextAreaControl& WithDisplayOptions(const TextAreaControlDisplayOptions& value) { SetDisplayOptions(value); return *this;}

    /**
     * <p>The display options of a control.</p>
     */
    inline FilterTextAreaControl& WithDisplayOptions(TextAreaControlDisplayOptions&& value) { SetDisplayOptions(std::move(value)); return *this;}

  private:

    Aws::String m_filterControlId;
    bool m_filterControlIdHasBeenSet = false;

    Aws::String m_title;
    bool m_titleHasBeenSet = false;

    Aws::String m_sourceFilterId;
    bool m_sourceFilterIdHasBeenSet = false;

    Aws::String m_delimiter;
    bool m_delimiterHasBeenSet = false;

    TextAreaControlDisplayOptions m_displayOptions;
    bool m_displayOptionsHasBeenSet = false;
  };

} // namespace Model
} // namespace QuickSight
} // namespace Aws
