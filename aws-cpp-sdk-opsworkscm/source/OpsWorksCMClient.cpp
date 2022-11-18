﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/opsworkscm/OpsWorksCMClient.h>
#include <aws/opsworkscm/OpsWorksCMErrorMarshaller.h>
#include <aws/opsworkscm/OpsWorksCMEndpointProvider.h>
#include <aws/opsworkscm/model/AssociateNodeRequest.h>
#include <aws/opsworkscm/model/CreateBackupRequest.h>
#include <aws/opsworkscm/model/CreateServerRequest.h>
#include <aws/opsworkscm/model/DeleteBackupRequest.h>
#include <aws/opsworkscm/model/DeleteServerRequest.h>
#include <aws/opsworkscm/model/DescribeAccountAttributesRequest.h>
#include <aws/opsworkscm/model/DescribeBackupsRequest.h>
#include <aws/opsworkscm/model/DescribeEventsRequest.h>
#include <aws/opsworkscm/model/DescribeNodeAssociationStatusRequest.h>
#include <aws/opsworkscm/model/DescribeServersRequest.h>
#include <aws/opsworkscm/model/DisassociateNodeRequest.h>
#include <aws/opsworkscm/model/ExportServerEngineAttributeRequest.h>
#include <aws/opsworkscm/model/ListTagsForResourceRequest.h>
#include <aws/opsworkscm/model/RestoreServerRequest.h>
#include <aws/opsworkscm/model/StartMaintenanceRequest.h>
#include <aws/opsworkscm/model/TagResourceRequest.h>
#include <aws/opsworkscm/model/UntagResourceRequest.h>
#include <aws/opsworkscm/model/UpdateServerRequest.h>
#include <aws/opsworkscm/model/UpdateServerEngineAttributesRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::OpsWorksCM;
using namespace Aws::OpsWorksCM::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* OpsWorksCMClient::SERVICE_NAME = "opsworks-cm";
const char* OpsWorksCMClient::ALLOCATION_TAG = "OpsWorksCMClient";

OpsWorksCMClient::OpsWorksCMClient(const OpsWorksCM::OpsWorksCMClientConfiguration& clientConfiguration,
                                   std::shared_ptr<OpsWorksCMEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<OpsWorksCMErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

OpsWorksCMClient::OpsWorksCMClient(const AWSCredentials& credentials,
                                   std::shared_ptr<OpsWorksCMEndpointProviderBase> endpointProvider,
                                   const OpsWorksCM::OpsWorksCMClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<OpsWorksCMErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

OpsWorksCMClient::OpsWorksCMClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   std::shared_ptr<OpsWorksCMEndpointProviderBase> endpointProvider,
                                   const OpsWorksCM::OpsWorksCMClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<OpsWorksCMErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  OpsWorksCMClient::OpsWorksCMClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<OpsWorksCMErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<OpsWorksCMEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

OpsWorksCMClient::OpsWorksCMClient(const AWSCredentials& credentials,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<OpsWorksCMErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<OpsWorksCMEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

OpsWorksCMClient::OpsWorksCMClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                   const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<OpsWorksCMErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<OpsWorksCMEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
OpsWorksCMClient::~OpsWorksCMClient()
{
}

std::shared_ptr<OpsWorksCMEndpointProviderBase>& OpsWorksCMClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void OpsWorksCMClient::init(const OpsWorksCM::OpsWorksCMClientConfiguration& config)
{
  AWSClient::SetServiceClientName("OpsWorksCM");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void OpsWorksCMClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateNodeOutcome OpsWorksCMClient::AssociateNode(const AssociateNodeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateNode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateNode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return AssociateNodeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateNodeOutcomeCallable OpsWorksCMClient::AssociateNodeCallable(const AssociateNodeRequest& request) const
{
  std::shared_ptr<AssociateNodeRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< AssociateNodeOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->AssociateNode(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::AssociateNodeAsync(const AssociateNodeRequest& request, const AssociateNodeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<AssociateNodeRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, AssociateNode(*pRequest), context);
    } );
}

CreateBackupOutcome OpsWorksCMClient::CreateBackup(const CreateBackupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateBackup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateBackup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateBackupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateBackupOutcomeCallable OpsWorksCMClient::CreateBackupCallable(const CreateBackupRequest& request) const
{
  std::shared_ptr<CreateBackupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateBackupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateBackup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::CreateBackupAsync(const CreateBackupRequest& request, const CreateBackupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateBackupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateBackup(*pRequest), context);
    } );
}

CreateServerOutcome OpsWorksCMClient::CreateServer(const CreateServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateServerOutcomeCallable OpsWorksCMClient::CreateServerCallable(const CreateServerRequest& request) const
{
  std::shared_ptr<CreateServerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateServerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateServer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::CreateServerAsync(const CreateServerRequest& request, const CreateServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateServerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateServer(*pRequest), context);
    } );
}

DeleteBackupOutcome OpsWorksCMClient::DeleteBackup(const DeleteBackupRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteBackup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteBackup, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteBackupOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteBackupOutcomeCallable OpsWorksCMClient::DeleteBackupCallable(const DeleteBackupRequest& request) const
{
  std::shared_ptr<DeleteBackupRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteBackupOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteBackup(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DeleteBackupAsync(const DeleteBackupRequest& request, const DeleteBackupResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteBackupRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteBackup(*pRequest), context);
    } );
}

DeleteServerOutcome OpsWorksCMClient::DeleteServer(const DeleteServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteServerOutcomeCallable OpsWorksCMClient::DeleteServerCallable(const DeleteServerRequest& request) const
{
  std::shared_ptr<DeleteServerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteServerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteServer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DeleteServerAsync(const DeleteServerRequest& request, const DeleteServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteServerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteServer(*pRequest), context);
    } );
}

DescribeAccountAttributesOutcome OpsWorksCMClient::DescribeAccountAttributes(const DescribeAccountAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeAccountAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeAccountAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeAccountAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeAccountAttributesOutcomeCallable OpsWorksCMClient::DescribeAccountAttributesCallable(const DescribeAccountAttributesRequest& request) const
{
  std::shared_ptr<DescribeAccountAttributesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeAccountAttributesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeAccountAttributes(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DescribeAccountAttributesAsync(const DescribeAccountAttributesRequest& request, const DescribeAccountAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeAccountAttributesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeAccountAttributes(*pRequest), context);
    } );
}

DescribeBackupsOutcome OpsWorksCMClient::DescribeBackups(const DescribeBackupsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeBackups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeBackups, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeBackupsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeBackupsOutcomeCallable OpsWorksCMClient::DescribeBackupsCallable(const DescribeBackupsRequest& request) const
{
  std::shared_ptr<DescribeBackupsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeBackupsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeBackups(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DescribeBackupsAsync(const DescribeBackupsRequest& request, const DescribeBackupsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeBackupsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeBackups(*pRequest), context);
    } );
}

DescribeEventsOutcome OpsWorksCMClient::DescribeEvents(const DescribeEventsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeEvents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeEventsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeEventsOutcomeCallable OpsWorksCMClient::DescribeEventsCallable(const DescribeEventsRequest& request) const
{
  std::shared_ptr<DescribeEventsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeEventsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeEvents(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DescribeEventsAsync(const DescribeEventsRequest& request, const DescribeEventsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeEventsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeEvents(*pRequest), context);
    } );
}

DescribeNodeAssociationStatusOutcome OpsWorksCMClient::DescribeNodeAssociationStatus(const DescribeNodeAssociationStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeNodeAssociationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeNodeAssociationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeNodeAssociationStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeNodeAssociationStatusOutcomeCallable OpsWorksCMClient::DescribeNodeAssociationStatusCallable(const DescribeNodeAssociationStatusRequest& request) const
{
  std::shared_ptr<DescribeNodeAssociationStatusRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeNodeAssociationStatusOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeNodeAssociationStatus(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DescribeNodeAssociationStatusAsync(const DescribeNodeAssociationStatusRequest& request, const DescribeNodeAssociationStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeNodeAssociationStatusRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeNodeAssociationStatus(*pRequest), context);
    } );
}

DescribeServersOutcome OpsWorksCMClient::DescribeServers(const DescribeServersRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeServers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeServers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeServersOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeServersOutcomeCallable OpsWorksCMClient::DescribeServersCallable(const DescribeServersRequest& request) const
{
  std::shared_ptr<DescribeServersRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeServersOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeServers(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DescribeServersAsync(const DescribeServersRequest& request, const DescribeServersResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeServersRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeServers(*pRequest), context);
    } );
}

DisassociateNodeOutcome OpsWorksCMClient::DisassociateNode(const DisassociateNodeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateNode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateNode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DisassociateNodeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DisassociateNodeOutcomeCallable OpsWorksCMClient::DisassociateNodeCallable(const DisassociateNodeRequest& request) const
{
  std::shared_ptr<DisassociateNodeRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DisassociateNodeOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DisassociateNode(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::DisassociateNodeAsync(const DisassociateNodeRequest& request, const DisassociateNodeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DisassociateNodeRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DisassociateNode(*pRequest), context);
    } );
}

ExportServerEngineAttributeOutcome OpsWorksCMClient::ExportServerEngineAttribute(const ExportServerEngineAttributeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ExportServerEngineAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ExportServerEngineAttribute, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ExportServerEngineAttributeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ExportServerEngineAttributeOutcomeCallable OpsWorksCMClient::ExportServerEngineAttributeCallable(const ExportServerEngineAttributeRequest& request) const
{
  std::shared_ptr<ExportServerEngineAttributeRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ExportServerEngineAttributeOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ExportServerEngineAttribute(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::ExportServerEngineAttributeAsync(const ExportServerEngineAttributeRequest& request, const ExportServerEngineAttributeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ExportServerEngineAttributeRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ExportServerEngineAttribute(*pRequest), context);
    } );
}

ListTagsForResourceOutcome OpsWorksCMClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable OpsWorksCMClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListTagsForResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListTagsForResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListTagsForResource(*pRequest), context);
    } );
}

RestoreServerOutcome OpsWorksCMClient::RestoreServer(const RestoreServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestoreServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestoreServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RestoreServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RestoreServerOutcomeCallable OpsWorksCMClient::RestoreServerCallable(const RestoreServerRequest& request) const
{
  std::shared_ptr<RestoreServerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RestoreServerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RestoreServer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::RestoreServerAsync(const RestoreServerRequest& request, const RestoreServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RestoreServerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RestoreServer(*pRequest), context);
    } );
}

StartMaintenanceOutcome OpsWorksCMClient::StartMaintenance(const StartMaintenanceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartMaintenance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartMaintenance, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return StartMaintenanceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartMaintenanceOutcomeCallable OpsWorksCMClient::StartMaintenanceCallable(const StartMaintenanceRequest& request) const
{
  std::shared_ptr<StartMaintenanceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< StartMaintenanceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->StartMaintenance(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::StartMaintenanceAsync(const StartMaintenanceRequest& request, const StartMaintenanceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<StartMaintenanceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, StartMaintenance(*pRequest), context);
    } );
}

TagResourceOutcome OpsWorksCMClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable OpsWorksCMClient::TagResourceCallable(const TagResourceRequest& request) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< TagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->TagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, TagResource(*pRequest), context);
    } );
}

UntagResourceOutcome OpsWorksCMClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable OpsWorksCMClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UntagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UntagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UntagResource(*pRequest), context);
    } );
}

UpdateServerOutcome OpsWorksCMClient::UpdateServer(const UpdateServerRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateServer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateServerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateServerOutcomeCallable OpsWorksCMClient::UpdateServerCallable(const UpdateServerRequest& request) const
{
  std::shared_ptr<UpdateServerRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateServerOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateServer(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::UpdateServerAsync(const UpdateServerRequest& request, const UpdateServerResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateServerRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateServer(*pRequest), context);
    } );
}

UpdateServerEngineAttributesOutcome OpsWorksCMClient::UpdateServerEngineAttributes(const UpdateServerEngineAttributesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateServerEngineAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateServerEngineAttributes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateServerEngineAttributesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateServerEngineAttributesOutcomeCallable OpsWorksCMClient::UpdateServerEngineAttributesCallable(const UpdateServerEngineAttributesRequest& request) const
{
  std::shared_ptr<UpdateServerEngineAttributesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateServerEngineAttributesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateServerEngineAttributes(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void OpsWorksCMClient::UpdateServerEngineAttributesAsync(const UpdateServerEngineAttributesRequest& request, const UpdateServerEngineAttributesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateServerEngineAttributesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateServerEngineAttributes(*pRequest), context);
    } );
}

