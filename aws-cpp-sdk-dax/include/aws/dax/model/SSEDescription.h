﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/dax/DAX_EXPORTS.h>
#include <aws/dax/model/SSEStatus.h>
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
namespace DAX
{
namespace Model
{

  /**
   * <p>The description of the server-side encryption status on the specified DAX
   * cluster.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/dax-2017-04-19/SSEDescription">AWS
   * API Reference</a></p>
   */
  class AWS_DAX_API SSEDescription
  {
  public:
    SSEDescription();
    SSEDescription(Aws::Utils::Json::JsonView jsonValue);
    SSEDescription& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The current state of server-side encryption:</p> <ul> <li> <p>
     * <code>ENABLING</code> - Server-side encryption is being enabled.</p> </li> <li>
     * <p> <code>ENABLED</code> - Server-side encryption is enabled.</p> </li> <li> <p>
     * <code>DISABLING</code> - Server-side encryption is being disabled.</p> </li>
     * <li> <p> <code>DISABLED</code> - Server-side encryption is disabled.</p> </li>
     * </ul>
     */
    inline const SSEStatus& GetStatus() const{ return m_status; }

    /**
     * <p>The current state of server-side encryption:</p> <ul> <li> <p>
     * <code>ENABLING</code> - Server-side encryption is being enabled.</p> </li> <li>
     * <p> <code>ENABLED</code> - Server-side encryption is enabled.</p> </li> <li> <p>
     * <code>DISABLING</code> - Server-side encryption is being disabled.</p> </li>
     * <li> <p> <code>DISABLED</code> - Server-side encryption is disabled.</p> </li>
     * </ul>
     */
    inline bool StatusHasBeenSet() const { return m_statusHasBeenSet; }

    /**
     * <p>The current state of server-side encryption:</p> <ul> <li> <p>
     * <code>ENABLING</code> - Server-side encryption is being enabled.</p> </li> <li>
     * <p> <code>ENABLED</code> - Server-side encryption is enabled.</p> </li> <li> <p>
     * <code>DISABLING</code> - Server-side encryption is being disabled.</p> </li>
     * <li> <p> <code>DISABLED</code> - Server-side encryption is disabled.</p> </li>
     * </ul>
     */
    inline void SetStatus(const SSEStatus& value) { m_statusHasBeenSet = true; m_status = value; }

    /**
     * <p>The current state of server-side encryption:</p> <ul> <li> <p>
     * <code>ENABLING</code> - Server-side encryption is being enabled.</p> </li> <li>
     * <p> <code>ENABLED</code> - Server-side encryption is enabled.</p> </li> <li> <p>
     * <code>DISABLING</code> - Server-side encryption is being disabled.</p> </li>
     * <li> <p> <code>DISABLED</code> - Server-side encryption is disabled.</p> </li>
     * </ul>
     */
    inline void SetStatus(SSEStatus&& value) { m_statusHasBeenSet = true; m_status = std::move(value); }

    /**
     * <p>The current state of server-side encryption:</p> <ul> <li> <p>
     * <code>ENABLING</code> - Server-side encryption is being enabled.</p> </li> <li>
     * <p> <code>ENABLED</code> - Server-side encryption is enabled.</p> </li> <li> <p>
     * <code>DISABLING</code> - Server-side encryption is being disabled.</p> </li>
     * <li> <p> <code>DISABLED</code> - Server-side encryption is disabled.</p> </li>
     * </ul>
     */
    inline SSEDescription& WithStatus(const SSEStatus& value) { SetStatus(value); return *this;}

    /**
     * <p>The current state of server-side encryption:</p> <ul> <li> <p>
     * <code>ENABLING</code> - Server-side encryption is being enabled.</p> </li> <li>
     * <p> <code>ENABLED</code> - Server-side encryption is enabled.</p> </li> <li> <p>
     * <code>DISABLING</code> - Server-side encryption is being disabled.</p> </li>
     * <li> <p> <code>DISABLED</code> - Server-side encryption is disabled.</p> </li>
     * </ul>
     */
    inline SSEDescription& WithStatus(SSEStatus&& value) { SetStatus(std::move(value)); return *this;}

  private:

    SSEStatus m_status;
    bool m_statusHasBeenSet = false;
  };

} // namespace Model
} // namespace DAX
} // namespace Aws
