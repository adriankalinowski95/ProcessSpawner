import { Component } from '@angular/core';
import { MainConfig } from '../../../../shared/element-list/models/ElementListConfig';
import { DataSource } from '../../../../shared/element-list/models/data-source';
import { DisplayedColumns } from '../../../../shared/element-list/models/displayed-columns';
import { defaultColumns, defaultConfig, ELEMENT_DATA } from '../../../../shared/element-list/models/defaultConfig';
import { HandelContentEvent } from '../../../../shared/element-list/models/HandelContentEvent';

@Component({
  selector: 'app-process-list',
  templateUrl: './process-list.component.html',
  styleUrl: './process-list.component.scss'
})
export class ProcessListComponent {
    /*
    readonly columnMap = Object.freeze({
        id: {
          label: 'ID',
          description: 'ID',
        },
        name: {
          label: 'Name',
          description: 'Name',
        },
        email: {
          label: 'Email',
          description: 'Email',
        },
        actions: {
          label: '',
          description: 'Actions',
        },
      });
    
      displayedColumns = Object.keys(this.columnMap);
    
      dataSource = [
        { id: 1, name: 'Fulano', email: 'fulano@mail.com' }
      ];
    */

    config: MainConfig = defaultConfig;
    dataSource: DataSource[] = ELEMENT_DATA;
    displayedColumns: DisplayedColumns = [...defaultColumns]
    
    testF($event: HandelContentEvent) {
        console.log('testF', $event)
    }
}
