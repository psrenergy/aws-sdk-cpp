﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/fms/FMS_EXPORTS.h>
#include <aws/fms/FMSRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <utility>

namespace Aws
{
namespace FMS
{
namespace Model
{

  /**
   */
  class AWS_FMS_API BatchDisassociateResourceRequest : public FMSRequest
  {
  public:
    BatchDisassociateResourceRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<BatchDisassociateResourceRequest> Clone() const
    {
      return Aws::MakeUnique<BatchDisassociateResourceRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "BatchDisassociateResource"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline const Aws::String& GetResourceSetIdentifier() const{ return m_resourceSetIdentifier; }

    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline bool ResourceSetIdentifierHasBeenSet() const { return m_resourceSetIdentifierHasBeenSet; }

    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline void SetResourceSetIdentifier(const Aws::String& value) { m_resourceSetIdentifierHasBeenSet = true; m_resourceSetIdentifier = value; }

    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline void SetResourceSetIdentifier(Aws::String&& value) { m_resourceSetIdentifierHasBeenSet = true; m_resourceSetIdentifier = std::move(value); }

    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline void SetResourceSetIdentifier(const char* value) { m_resourceSetIdentifierHasBeenSet = true; m_resourceSetIdentifier.assign(value); }

    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline BatchDisassociateResourceRequest& WithResourceSetIdentifier(const Aws::String& value) { SetResourceSetIdentifier(value); return *this;}

    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline BatchDisassociateResourceRequest& WithResourceSetIdentifier(Aws::String&& value) { SetResourceSetIdentifier(std::move(value)); return *this;}

    /**
     * <p>A unique identifier for the resource set, used in a TODO to refer to the
     * resource set.</p>
     */
    inline BatchDisassociateResourceRequest& WithResourceSetIdentifier(const char* value) { SetResourceSetIdentifier(value); return *this;}


    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline const Aws::Vector<Aws::String>& GetItems() const{ return m_items; }

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline bool ItemsHasBeenSet() const { return m_itemsHasBeenSet; }

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline void SetItems(const Aws::Vector<Aws::String>& value) { m_itemsHasBeenSet = true; m_items = value; }

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline void SetItems(Aws::Vector<Aws::String>&& value) { m_itemsHasBeenSet = true; m_items = std::move(value); }

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline BatchDisassociateResourceRequest& WithItems(const Aws::Vector<Aws::String>& value) { SetItems(value); return *this;}

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline BatchDisassociateResourceRequest& WithItems(Aws::Vector<Aws::String>&& value) { SetItems(std::move(value)); return *this;}

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline BatchDisassociateResourceRequest& AddItems(const Aws::String& value) { m_itemsHasBeenSet = true; m_items.push_back(value); return *this; }

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline BatchDisassociateResourceRequest& AddItems(Aws::String&& value) { m_itemsHasBeenSet = true; m_items.push_back(std::move(value)); return *this; }

    /**
     * <p>The uniform resource identifiers (URI) of resources that should be
     * disassociated from the resource set. The URIs must be Amazon Resource Names
     * (ARNs).</p>
     */
    inline BatchDisassociateResourceRequest& AddItems(const char* value) { m_itemsHasBeenSet = true; m_items.push_back(value); return *this; }

  private:

    Aws::String m_resourceSetIdentifier;
    bool m_resourceSetIdentifierHasBeenSet = false;

    Aws::Vector<Aws::String> m_items;
    bool m_itemsHasBeenSet = false;
  };

} // namespace Model
} // namespace FMS
} // namespace Aws
