import { Component, input, Input, OnInit, signal } from '@angular/core';
import { MenuItem } from '../../models/menu-item';
import { animate, style, transition, trigger } from '@angular/animations';

@Component({
  selector: 'app-sidenav-menu-item',
  templateUrl: './sidenav-menu-item.component.html',
  styleUrl: './sidenav-menu-item.component.scss',
  animations: [
    trigger('expandedContractMenu',[
        transition(':enter',[
            style({opacity: 0, height: '0px'}),
            animate('500ms ease-in-out', style({opacity: 1, height: '*'}))
        ]),
        transition(':leave',[
            animate('500ms ease-in-out', style({opacity: 0, height: '0px'}))
        ])
    ])
  ]
})
export class SidenavMenuItemComponent implements OnInit {
    @Input({ required: true })
    item: MenuItem;

    @Input()
    isExpanded: boolean = true;

    @Input()
    isSubItem: boolean = false;

    nestedMenuOpen = signal(false);

    ngOnInit(): void {
    }

    toggleNested() {
        if (!this.item.subItems) {
            return;
        }

        this.nestedMenuOpen.set(!this.nestedMenuOpen());
    }

    getSelectedItemClass() {
        return !this.isSubItem ? 'selected-menu-item' : 'non-selected-menu-item'
    }
}
