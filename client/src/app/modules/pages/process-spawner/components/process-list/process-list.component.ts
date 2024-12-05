import { Component } from '@angular/core';
import { MainConfig } from '../../../../shared/element-list/models/ElementListConfig';
import { DataSource } from '../../../../shared/element-list/models/data-source';
import { DisplayedColumns } from '../../../../shared/element-list/models/displayed-columns';
import { defaultColumns, defaultConfig, ELEMENT_DATA } from '../../../../shared/element-list/models/defaultConfig';
import { HandelContentEvent } from '../../../../shared/element-list/models/HandelContentEvent';
import { SpawnProcessCrudService } from '../../services/spawn-process-commuincation.service';
import { ActionType } from '../../../../shared/action-button/models/action-button-config';
import { SpawnProcessRequestDto } from '../../models/spawn-process-request-dto';
import { catchError, first, of, tap } from 'rxjs';
import { shared } from '../../../../shared/shared';
import { SpawnProcessResponseDto } from '../../models/spawn-process-response-dto';

@Component({
  selector: 'app-process-list',
  templateUrl: './process-list.component.html',
  styleUrl: './process-list.component.scss'
})
export class ProcessListComponent {
    private columns: DisplayedColumns = [
        {name: 'index', displayName: 'No.'},
        {name: 'name', displayName: 'Name'},
        {name: 'weight', displayName: 'Weight'},
        {name: 'symbol', displayName: 'Symbol'}
    ];
      
    private elements: DataSource[] = [
        {index: 1, name: 'Hydrogen', weight: 1.0079, symbol: 'H'},
        {index: 2, name: 'Helium', weight: 4.0026, symbol: 'He'},
        {index: 3, name: 'Lithium', weight: 6.941, symbol: 'Li'},
        {index: 4, name: 'Beryllium', weight: 9.0122, symbol: 'Be'},
        {index: 5, name: 'Boron', weight: 10.811, symbol: 'B'},
        {index: 6, name: 'Carbon', weight: 12.0107, symbol: 'C'},
        {index: 7, name: 'Nitrogen', weight: 14.0067, symbol: 'N'},
        {index: 8, name: 'Oxygen', weight: 15.9994, symbol: 'O'},
        {index: 9, name: 'Fluorine', weight: 18.9984, symbol: 'F'},
        {index: 10, name: 'Neon', weight: 20.1797, symbol: 'Ne'},
        {index: 11, name: 'Hydrogen', weight: 1.0079, symbol: 'H'},
        {index: 12, name: 'Helium', weight: 4.0026, symbol: 'He'},
        {index: 13, name: 'Lithium', weight: 6.941, symbol: 'Li'},
        {index: 14, name: 'Beryllium', weight: 9.0122, symbol: 'Be'},
        {index: 15, name: 'Boron', weight: 10.811, symbol: 'B'},
        {index: 16, name: 'Carbon', weight: 12.0107, symbol: 'C'},
        {index: 17, name: 'Nitrogen', weight: 14.0067, symbol: 'N'},
        {index: 18, name: 'Oxygen', weight: 15.9994, symbol: 'O'},
        {index: 19, name: 'Fluorine', weight: 18.9984, symbol: 'F'},
        {index: 20, name: 'Neon', weight: 20.1797, symbol: 'Ne'},
    ];
      

    config: MainConfig = defaultConfig;
    dataSource: DataSource[] = ELEMENT_DATA;
    displayedColumns: DisplayedColumns = [...defaultColumns]
    
    constructor(private spawnProcessCrudService: SpawnProcessCrudService) {}

    eventHandler($event: HandelContentEvent) {
        if ($event.actionType === ActionType.Create) {
            this.processSpawning();
        }
    }

    processSpawning() {
        const spawnProcessRequest : SpawnProcessRequestDto = {
            process_type: 'test',
            parameters: new Map<string, string>([
                ['param1', 'param1']
            ])
        };

        this.spawnProcessCrudService.spawnProcess(spawnProcessRequest).pipe(
            first(),
            tap((spawnProcessResponse: shared.response.Object<SpawnProcessResponseDto>) => {
                console.log(spawnProcessResponse);
            }),
            catchError((err) => {
                console.log(err.message);

                return of(undefined);
            }),
        ).subscribe();
    }
}
