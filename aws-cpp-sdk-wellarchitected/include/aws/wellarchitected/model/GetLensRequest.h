﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/wellarchitected/WellArchitected_EXPORTS.h>
#include <aws/wellarchitected/WellArchitectedRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Http
{
    class URI;
} //namespace Http
namespace WellArchitected
{
namespace Model
{

  /**
   */
  class AWS_WELLARCHITECTED_API GetLensRequest : public WellArchitectedRequest
  {
  public:
    GetLensRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetLensRequest> Clone() const
    {
      return Aws::MakeUnique<GetLensRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetLens"; }

    Aws::String SerializePayload() const override;

    void AddQueryStringParameters(Aws::Http::URI& uri) const override;


    
    inline const Aws::String& GetLensAlias() const{ return m_lensAlias; }

    
    inline bool LensAliasHasBeenSet() const { return m_lensAliasHasBeenSet; }

    
    inline void SetLensAlias(const Aws::String& value) { m_lensAliasHasBeenSet = true; m_lensAlias = value; }

    
    inline void SetLensAlias(Aws::String&& value) { m_lensAliasHasBeenSet = true; m_lensAlias = std::move(value); }

    
    inline void SetLensAlias(const char* value) { m_lensAliasHasBeenSet = true; m_lensAlias.assign(value); }

    
    inline GetLensRequest& WithLensAlias(const Aws::String& value) { SetLensAlias(value); return *this;}

    
    inline GetLensRequest& WithLensAlias(Aws::String&& value) { SetLensAlias(std::move(value)); return *this;}

    
    inline GetLensRequest& WithLensAlias(const char* value) { SetLensAlias(value); return *this;}


    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline const Aws::String& GetLensVersion() const{ return m_lensVersion; }

    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline bool LensVersionHasBeenSet() const { return m_lensVersionHasBeenSet; }

    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline void SetLensVersion(const Aws::String& value) { m_lensVersionHasBeenSet = true; m_lensVersion = value; }

    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline void SetLensVersion(Aws::String&& value) { m_lensVersionHasBeenSet = true; m_lensVersion = std::move(value); }

    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline void SetLensVersion(const char* value) { m_lensVersionHasBeenSet = true; m_lensVersion.assign(value); }

    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline GetLensRequest& WithLensVersion(const Aws::String& value) { SetLensVersion(value); return *this;}

    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline GetLensRequest& WithLensVersion(Aws::String&& value) { SetLensVersion(std::move(value)); return *this;}

    /**
     * <p>The lens version to be retrieved.</p>
     */
    inline GetLensRequest& WithLensVersion(const char* value) { SetLensVersion(value); return *this;}

  private:

    Aws::String m_lensAlias;
    bool m_lensAliasHasBeenSet = false;

    Aws::String m_lensVersion;
    bool m_lensVersionHasBeenSet = false;
  };

} // namespace Model
} // namespace WellArchitected
} // namespace Aws
