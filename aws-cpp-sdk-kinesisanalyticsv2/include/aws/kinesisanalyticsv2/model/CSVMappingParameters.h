﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/kinesisanalyticsv2/KinesisAnalyticsV2_EXPORTS.h>
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
namespace KinesisAnalyticsV2
{
namespace Model
{

  /**
   * <p>For a SQL-based Kinesis Data Analytics application, provides additional
   * mapping information when the record format uses delimiters, such as CSV. For
   * example, the following sample records use CSV format, where the records use the
   * <i>'\n'</i> as the row delimiter and a comma (",") as the column delimiter: </p>
   * <p> <code>"name1", "address1"</code> </p> <p> <code>"name2", "address2"</code>
   * </p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/kinesisanalyticsv2-2018-05-23/CSVMappingParameters">AWS
   * API Reference</a></p>
   */
  class AWS_KINESISANALYTICSV2_API CSVMappingParameters
  {
  public:
    CSVMappingParameters();
    CSVMappingParameters(Aws::Utils::Json::JsonView jsonValue);
    CSVMappingParameters& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline const Aws::String& GetRecordRowDelimiter() const{ return m_recordRowDelimiter; }

    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline bool RecordRowDelimiterHasBeenSet() const { return m_recordRowDelimiterHasBeenSet; }

    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline void SetRecordRowDelimiter(const Aws::String& value) { m_recordRowDelimiterHasBeenSet = true; m_recordRowDelimiter = value; }

    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline void SetRecordRowDelimiter(Aws::String&& value) { m_recordRowDelimiterHasBeenSet = true; m_recordRowDelimiter = std::move(value); }

    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline void SetRecordRowDelimiter(const char* value) { m_recordRowDelimiterHasBeenSet = true; m_recordRowDelimiter.assign(value); }

    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline CSVMappingParameters& WithRecordRowDelimiter(const Aws::String& value) { SetRecordRowDelimiter(value); return *this;}

    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline CSVMappingParameters& WithRecordRowDelimiter(Aws::String&& value) { SetRecordRowDelimiter(std::move(value)); return *this;}

    /**
     * <p>The row delimiter. For example, in a CSV format, <i>'\n'</i> is the typical
     * row delimiter.</p>
     */
    inline CSVMappingParameters& WithRecordRowDelimiter(const char* value) { SetRecordRowDelimiter(value); return *this;}


    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline const Aws::String& GetRecordColumnDelimiter() const{ return m_recordColumnDelimiter; }

    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline bool RecordColumnDelimiterHasBeenSet() const { return m_recordColumnDelimiterHasBeenSet; }

    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline void SetRecordColumnDelimiter(const Aws::String& value) { m_recordColumnDelimiterHasBeenSet = true; m_recordColumnDelimiter = value; }

    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline void SetRecordColumnDelimiter(Aws::String&& value) { m_recordColumnDelimiterHasBeenSet = true; m_recordColumnDelimiter = std::move(value); }

    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline void SetRecordColumnDelimiter(const char* value) { m_recordColumnDelimiterHasBeenSet = true; m_recordColumnDelimiter.assign(value); }

    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline CSVMappingParameters& WithRecordColumnDelimiter(const Aws::String& value) { SetRecordColumnDelimiter(value); return *this;}

    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline CSVMappingParameters& WithRecordColumnDelimiter(Aws::String&& value) { SetRecordColumnDelimiter(std::move(value)); return *this;}

    /**
     * <p>The column delimiter. For example, in a CSV format, a comma (",") is the
     * typical column delimiter.</p>
     */
    inline CSVMappingParameters& WithRecordColumnDelimiter(const char* value) { SetRecordColumnDelimiter(value); return *this;}

  private:

    Aws::String m_recordRowDelimiter;
    bool m_recordRowDelimiterHasBeenSet = false;

    Aws::String m_recordColumnDelimiter;
    bool m_recordColumnDelimiterHasBeenSet = false;
  };

} // namespace Model
} // namespace KinesisAnalyticsV2
} // namespace Aws
