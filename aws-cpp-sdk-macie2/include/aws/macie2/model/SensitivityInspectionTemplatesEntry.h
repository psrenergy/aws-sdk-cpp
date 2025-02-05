﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/macie2/Macie2_EXPORTS.h>
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
namespace Macie2
{
namespace Model
{

  /**
   * <p>Provides information about the sensitivity inspection template for an Amazon
   * Macie account. Macie uses the template's settings when it performs automated
   * sensitive data discovery for the account.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/macie2-2020-01-01/SensitivityInspectionTemplatesEntry">AWS
   * API Reference</a></p>
   */
  class AWS_MACIE2_API SensitivityInspectionTemplatesEntry
  {
  public:
    SensitivityInspectionTemplatesEntry();
    SensitivityInspectionTemplatesEntry(Aws::Utils::Json::JsonView jsonValue);
    SensitivityInspectionTemplatesEntry& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline const Aws::String& GetId() const{ return m_id; }

    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline bool IdHasBeenSet() const { return m_idHasBeenSet; }

    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline void SetId(const Aws::String& value) { m_idHasBeenSet = true; m_id = value; }

    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline void SetId(Aws::String&& value) { m_idHasBeenSet = true; m_id = std::move(value); }

    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline void SetId(const char* value) { m_idHasBeenSet = true; m_id.assign(value); }

    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline SensitivityInspectionTemplatesEntry& WithId(const Aws::String& value) { SetId(value); return *this;}

    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline SensitivityInspectionTemplatesEntry& WithId(Aws::String&& value) { SetId(std::move(value)); return *this;}

    /**
     * <p>The unique identifier for the sensitivity inspection template for the
     * account.</p>
     */
    inline SensitivityInspectionTemplatesEntry& WithId(const char* value) { SetId(value); return *this;}


    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline const Aws::String& GetName() const{ return m_name; }

    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline bool NameHasBeenSet() const { return m_nameHasBeenSet; }

    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline void SetName(const Aws::String& value) { m_nameHasBeenSet = true; m_name = value; }

    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline void SetName(Aws::String&& value) { m_nameHasBeenSet = true; m_name = std::move(value); }

    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline void SetName(const char* value) { m_nameHasBeenSet = true; m_name.assign(value); }

    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline SensitivityInspectionTemplatesEntry& WithName(const Aws::String& value) { SetName(value); return *this;}

    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline SensitivityInspectionTemplatesEntry& WithName(Aws::String&& value) { SetName(std::move(value)); return *this;}

    /**
     * <p>The name of the sensitivity inspection template for the account.</p>
     */
    inline SensitivityInspectionTemplatesEntry& WithName(const char* value) { SetName(value); return *this;}

  private:

    Aws::String m_id;
    bool m_idHasBeenSet = false;

    Aws::String m_name;
    bool m_nameHasBeenSet = false;
  };

} // namespace Model
} // namespace Macie2
} // namespace Aws
