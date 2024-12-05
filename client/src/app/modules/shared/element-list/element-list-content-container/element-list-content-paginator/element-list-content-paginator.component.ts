import {Component, Input} from '@angular/core';
import { PaginationConfig } from '../../models/ElementListConfig';

@Component({
  selector: 'app-element-list-content-paginator',
  templateUrl: './element-list-content-paginator.component.html',
  styleUrl: './element-list-content-paginator.component.scss'
})
export class ElementListContentPaginatorComponent {
  @Input() paginationConfig: PaginationConfig;

}
