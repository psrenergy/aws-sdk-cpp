﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/devicefarm/DeviceFarm_EXPORTS.h>

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
namespace DeviceFarm
{
namespace Model
{

  /**
   * <p>Represents information about free trial device minutes for an AWS
   * account.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/devicefarm-2015-06-23/TrialMinutes">AWS
   * API Reference</a></p>
   */
  class AWS_DEVICEFARM_API TrialMinutes
  {
  public:
    TrialMinutes();
    TrialMinutes(Aws::Utils::Json::JsonView jsonValue);
    TrialMinutes& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The total number of free trial minutes that the account started with.</p>
     */
    inline double GetTotal() const{ return m_total; }

    /**
     * <p>The total number of free trial minutes that the account started with.</p>
     */
    inline bool TotalHasBeenSet() const { return m_totalHasBeenSet; }

    /**
     * <p>The total number of free trial minutes that the account started with.</p>
     */
    inline void SetTotal(double value) { m_totalHasBeenSet = true; m_total = value; }

    /**
     * <p>The total number of free trial minutes that the account started with.</p>
     */
    inline TrialMinutes& WithTotal(double value) { SetTotal(value); return *this;}


    /**
     * <p>The number of free trial minutes remaining in the account.</p>
     */
    inline double GetRemaining() const{ return m_remaining; }

    /**
     * <p>The number of free trial minutes remaining in the account.</p>
     */
    inline bool RemainingHasBeenSet() const { return m_remainingHasBeenSet; }

    /**
     * <p>The number of free trial minutes remaining in the account.</p>
     */
    inline void SetRemaining(double value) { m_remainingHasBeenSet = true; m_remaining = value; }

    /**
     * <p>The number of free trial minutes remaining in the account.</p>
     */
    inline TrialMinutes& WithRemaining(double value) { SetRemaining(value); return *this;}

  private:

    double m_total;
    bool m_totalHasBeenSet = false;

    double m_remaining;
    bool m_remainingHasBeenSet = false;
  };

} // namespace Model
} // namespace DeviceFarm
} // namespace Aws
