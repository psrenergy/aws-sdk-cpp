﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/mturk-requester/MTurk_EXPORTS.h>
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
namespace MTurk
{
namespace Model
{

  /**
   * <p>The Locale data structure represents a geographical region or
   * location.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/mturk-requester-2017-01-17/Locale">AWS
   * API Reference</a></p>
   */
  class AWS_MTURK_API Locale
  {
  public:
    Locale();
    Locale(Aws::Utils::Json::JsonView jsonValue);
    Locale& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline const Aws::String& GetCountry() const{ return m_country; }

    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline bool CountryHasBeenSet() const { return m_countryHasBeenSet; }

    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline void SetCountry(const Aws::String& value) { m_countryHasBeenSet = true; m_country = value; }

    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline void SetCountry(Aws::String&& value) { m_countryHasBeenSet = true; m_country = std::move(value); }

    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline void SetCountry(const char* value) { m_countryHasBeenSet = true; m_country.assign(value); }

    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline Locale& WithCountry(const Aws::String& value) { SetCountry(value); return *this;}

    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline Locale& WithCountry(Aws::String&& value) { SetCountry(std::move(value)); return *this;}

    /**
     * <p> The country of the locale. Must be a valid ISO 3166 country code. For
     * example, the code US refers to the United States of America. </p>
     */
    inline Locale& WithCountry(const char* value) { SetCountry(value); return *this;}


    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline const Aws::String& GetSubdivision() const{ return m_subdivision; }

    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline bool SubdivisionHasBeenSet() const { return m_subdivisionHasBeenSet; }

    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline void SetSubdivision(const Aws::String& value) { m_subdivisionHasBeenSet = true; m_subdivision = value; }

    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline void SetSubdivision(Aws::String&& value) { m_subdivisionHasBeenSet = true; m_subdivision = std::move(value); }

    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline void SetSubdivision(const char* value) { m_subdivisionHasBeenSet = true; m_subdivision.assign(value); }

    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline Locale& WithSubdivision(const Aws::String& value) { SetSubdivision(value); return *this;}

    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline Locale& WithSubdivision(Aws::String&& value) { SetSubdivision(std::move(value)); return *this;}

    /**
     * <p>The state or subdivision of the locale. A valid ISO 3166-2 subdivision code.
     * For example, the code WA refers to the state of Washington.</p>
     */
    inline Locale& WithSubdivision(const char* value) { SetSubdivision(value); return *this;}

  private:

    Aws::String m_country;
    bool m_countryHasBeenSet = false;

    Aws::String m_subdivision;
    bool m_subdivisionHasBeenSet = false;
  };

} // namespace Model
} // namespace MTurk
} // namespace Aws
