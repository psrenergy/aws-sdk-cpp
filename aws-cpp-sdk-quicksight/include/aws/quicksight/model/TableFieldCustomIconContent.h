﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/quicksight/QuickSight_EXPORTS.h>
#include <aws/quicksight/model/TableFieldIconSetType.h>
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
   * <p>The custom icon content for the table link content
   * configuration.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/quicksight-2018-04-01/TableFieldCustomIconContent">AWS
   * API Reference</a></p>
   */
  class AWS_QUICKSIGHT_API TableFieldCustomIconContent
  {
  public:
    TableFieldCustomIconContent();
    TableFieldCustomIconContent(Aws::Utils::Json::JsonView jsonValue);
    TableFieldCustomIconContent& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The icon set type (link) of the custom icon content for table URL link
     * content.</p>
     */
    inline const TableFieldIconSetType& GetIcon() const{ return m_icon; }

    /**
     * <p>The icon set type (link) of the custom icon content for table URL link
     * content.</p>
     */
    inline bool IconHasBeenSet() const { return m_iconHasBeenSet; }

    /**
     * <p>The icon set type (link) of the custom icon content for table URL link
     * content.</p>
     */
    inline void SetIcon(const TableFieldIconSetType& value) { m_iconHasBeenSet = true; m_icon = value; }

    /**
     * <p>The icon set type (link) of the custom icon content for table URL link
     * content.</p>
     */
    inline void SetIcon(TableFieldIconSetType&& value) { m_iconHasBeenSet = true; m_icon = std::move(value); }

    /**
     * <p>The icon set type (link) of the custom icon content for table URL link
     * content.</p>
     */
    inline TableFieldCustomIconContent& WithIcon(const TableFieldIconSetType& value) { SetIcon(value); return *this;}

    /**
     * <p>The icon set type (link) of the custom icon content for table URL link
     * content.</p>
     */
    inline TableFieldCustomIconContent& WithIcon(TableFieldIconSetType&& value) { SetIcon(std::move(value)); return *this;}

  private:

    TableFieldIconSetType m_icon;
    bool m_iconHasBeenSet = false;
  };

} // namespace Model
} // namespace QuickSight
} // namespace Aws
