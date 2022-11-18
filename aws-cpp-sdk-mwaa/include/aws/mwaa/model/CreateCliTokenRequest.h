﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/mwaa/MWAA_EXPORTS.h>
#include <aws/mwaa/MWAARequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace MWAA
{
namespace Model
{

  /**
   */
  class AWS_MWAA_API CreateCliTokenRequest : public MWAARequest
  {
  public:
    CreateCliTokenRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<CreateCliTokenRequest> Clone() const
    {
      return Aws::MakeUnique<CreateCliTokenRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreateCliToken"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline const Aws::String& GetName() const{ return m_name; }

    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline bool NameHasBeenSet() const { return m_nameHasBeenSet; }

    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline void SetName(const Aws::String& value) { m_nameHasBeenSet = true; m_name = value; }

    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline void SetName(Aws::String&& value) { m_nameHasBeenSet = true; m_name = std::move(value); }

    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline void SetName(const char* value) { m_nameHasBeenSet = true; m_name.assign(value); }

    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline CreateCliTokenRequest& WithName(const Aws::String& value) { SetName(value); return *this;}

    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline CreateCliTokenRequest& WithName(Aws::String&& value) { SetName(std::move(value)); return *this;}

    /**
     * <p>The name of the Amazon MWAA environment. For example,
     * <code>MyMWAAEnvironment</code>.</p>
     */
    inline CreateCliTokenRequest& WithName(const char* value) { SetName(value); return *this;}

  private:

    Aws::String m_name;
    bool m_nameHasBeenSet = false;
  };

} // namespace Model
} // namespace MWAA
} // namespace Aws
