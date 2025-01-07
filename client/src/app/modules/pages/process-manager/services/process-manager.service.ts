import { Injectable } from "@angular/core";
import { BaseCrudServiceImpl } from "../../../shared/request/base-crud-impl-service";
import { ProcessInstanceDto } from "../../process-spawner/models/process-instance-dto";
import { isProcessManagerDto, ProcessManagerDto } from "../models/process-manager-dto";
import { ProcessManagerCrudService } from "./process-manager-crud.service";
import { MatDialog } from "@angular/material/dialog";
import { first, map, Observable, of, switchMap, tap } from "rxjs";
import { DialogComponent } from "app/modules/shared/dialog/dialog.component";
import { DynamicFormComponent } from "app/modules/shared/dynamic-form/dynamic-form.component";
import { processManagerCreateConfig } from "../components/process-manager-form-config/process-manager-create-config";
import { ActionType, Type } from "app/modules/shared/action-button/models/action-button-config";
import { shared } from "app/modules/shared/shared";
import { ObjectResponse } from "app/modules/shared/response/object-response";
import { getEditConfig } from "../components/process-manager-form-config/process-manager-edit-config";

@Injectable({
    providedIn: 'root'
})
export class ProcessManagerService extends BaseCrudServiceImpl<ProcessManagerDto> {
    constructor(private dialog: MatDialog, private processManagerCrudService: ProcessManagerCrudService) 
    {
        super(processManagerCrudService);
    }

    createProcessManager() : Observable<ProcessManagerDto | undefined> {
       return this.dialog.open(DialogComponent<DynamicFormComponent>, {
                  width: '550px',
                  height: '550px',
                  data: {
                    name: 'createProcessManager',
                    header: 'Create process manager',
                    contentData: processManagerCreateConfig,
                    contentRef: DynamicFormComponent,
                    config: {
                      actionPanelButtons: [
                        {
                          disabled: false,
                          type: Type.Info,
                          text: 'Accept',
                          actionType: ActionType.Save
                        },
                        {
                          disabled: false,
                          type: Type.Error,
                          text: 'Close',
                          actionType: ActionType.Close
                        }
                      ]
                    }
                  }
                }).afterClosed().pipe(
                    first(),
                    switchMap(formResult => {
                        const processManagerDto = this.createProcessManagerDto(formResult);
                        if (!shared.isNotNullOrUndefined(processManagerDto)) {
                            return of(undefined);
                        }

                        if (!isProcessManagerDto(processManagerDto)) {
                            return of(undefined);
                        }

                        return this.create(processManagerDto);
                  }));
    }

    private createProcessManagerDto(formResult: any) : ProcessManagerDto | undefined {
        if (!shared.isNotNullOrUndefined(formResult)) {
            return undefined;
        }

        const haveAllRequiredFields = 'nameGroup' in formResult && 'networkGroup' in formResult;
        if (!haveAllRequiredFields) {
            return undefined;
        }

        const nameGroup = formResult['nameGroup'];
        const networkGroup = formResult['networkGroup'];

        const haveAllRequiredFieldsInNameGroup = 'name' in nameGroup && 'address' in networkGroup && 'port' in networkGroup;
        if (!haveAllRequiredFieldsInNameGroup) {
            return undefined;
        }

        return {
            id: 0,
            name: nameGroup['name'],
            address: networkGroup['address'],
            port: networkGroup['port']
        };
    }

    updateProcessManager(processManager: ProcessManagerDto) {
        return this.dialog.open(DialogComponent<DynamicFormComponent>, {
            width: '550px',
            height: '550px',
            data: {
              name: 'createProcessManager',
              header: 'Create process manager',
              contentData: getEditConfig(processManager),
              contentRef: DynamicFormComponent,
              config: {
                actionPanelButtons: [
                  {
                    disabled: false,
                    type: Type.Info,
                    text: 'Accept',
                    actionType: ActionType.Save
                  },
                  {
                    disabled: false,
                    type: Type.Error,
                    text: 'Close',
                    actionType: ActionType.Close
                  }
                ]
              }
            }
          }).afterClosed().pipe(
              first(),
              switchMap(formResult => {
                  let processManagerDto = this.createProcessManagerDto(formResult);
                  if (!shared.isNotNullOrUndefined(processManagerDto)) {
                      return of(undefined);
                  }

                  if (!isProcessManagerDto(processManagerDto)) {
                      return of(undefined);
                  }

                  processManagerDto.id = processManager.id;

                  return this.update(processManagerDto);
            }));
    }
}