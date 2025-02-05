﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/quicksight/QuickSight_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
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
   * <p>Formatting configuration for solid color.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/quicksight-2018-04-01/ConditionalFormattingSolidColor">AWS
   * API Reference</a></p>
   */
  class AWS_QUICKSIGHT_API ConditionalFormattingSolidColor
  {
  public:
    ConditionalFormattingSolidColor();
    ConditionalFormattingSolidColor(Aws::Utils::Json::JsonView jsonValue);
    ConditionalFormattingSolidColor& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline const Aws::String& GetExpression() const{ return m_expression; }

    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline bool ExpressionHasBeenSet() const { return m_expressionHasBeenSet; }

    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline void SetExpression(const Aws::String& value) { m_expressionHasBeenSet = true; m_expression = value; }

    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline void SetExpression(Aws::String&& value) { m_expressionHasBeenSet = true; m_expression = std::move(value); }

    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline void SetExpression(const char* value) { m_expressionHasBeenSet = true; m_expression.assign(value); }

    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline ConditionalFormattingSolidColor& WithExpression(const Aws::String& value) { SetExpression(value); return *this;}

    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline ConditionalFormattingSolidColor& WithExpression(Aws::String&& value) { SetExpression(std::move(value)); return *this;}

    /**
     * <p>The expression that determines the formatting configuration for solid
     * color.</p>
     */
    inline ConditionalFormattingSolidColor& WithExpression(const char* value) { SetExpression(value); return *this;}


    /**
     * <p>Determines the color.</p>
     */
    inline const Aws::String& GetColor() const{ return m_color; }

    /**
     * <p>Determines the color.</p>
     */
    inline bool ColorHasBeenSet() const { return m_colorHasBeenSet; }

    /**
     * <p>Determines the color.</p>
     */
    inline void SetColor(const Aws::String& value) { m_colorHasBeenSet = true; m_color = value; }

    /**
     * <p>Determines the color.</p>
     */
    inline void SetColor(Aws::String&& value) { m_colorHasBeenSet = true; m_color = std::move(value); }

    /**
     * <p>Determines the color.</p>
     */
    inline void SetColor(const char* value) { m_colorHasBeenSet = true; m_color.assign(value); }

    /**
     * <p>Determines the color.</p>
     */
    inline ConditionalFormattingSolidColor& WithColor(const Aws::String& value) { SetColor(value); return *this;}

    /**
     * <p>Determines the color.</p>
     */
    inline ConditionalFormattingSolidColor& WithColor(Aws::String&& value) { SetColor(std::move(value)); return *this;}

    /**
     * <p>Determines the color.</p>
     */
    inline ConditionalFormattingSolidColor& WithColor(const char* value) { SetColor(value); return *this;}

  private:

    Aws::String m_expression;
    bool m_expressionHasBeenSet = false;

    Aws::String m_color;
    bool m_colorHasBeenSet = false;
  };

} // namespace Model
} // namespace QuickSight
} // namespace Aws
