import { Component, Input, OnInit } from '@angular/core';
import { MenuItem } from '../../models/menu-item';
import { AuthService } from '../../../../../auth/services/auth.service';
import { isNotNullOrUndefined } from '../../../../../shared/utils/validators';
import { shared } from '../../../../../shared/shared';
import { Roles } from 'app/modules/auth/enums/roles';

@Component({
  selector: 'app-sidenav',
  templateUrl: './sidenav.component.html',
  styleUrl: './sidenav.component.scss'
})
export class SidenavComponent implements OnInit{
    private Default_Menu_Items: MenuItem[] = [
        {
            label: 'Dashboard',
            icon: 'dashboard',
            route: '/dashboard',
            role: []
        },
        {
            label: 'Process Spawner',
            icon: 'rocket_launch',
            route: '/process-spawner/user/',
            role: []
        },
        {
            label: 'Process Manager',
            icon: 'manage_accounts',
            route: '/process-manager',
            role: [ Roles.Admin ]
        }
    ];

    @Input() isExpanded: boolean = false;

    menuItems: MenuItem[] = this.Default_Menu_Items;

    constructor(public authService: AuthService) {}

    ngOnInit(): void {
        this.filterMenuItems();
    }

    private filterMenuItems() {
        const user = this.authService.getAuthFromLocalStorage()?.user;
        const token = this.authService.getAuthFromLocalStorage()?.token;
        if (!shared.isNotNullOrUndefined(user) || !shared.isNotNullOrUndefined(token)) {
            return;
        }

        let processSpawnerMenuItem = this.getProcessSpawnerMenuItem();
        if (shared.isNotNullOrUndefined(processSpawnerMenuItem)) {
            // @Todo repair a active route navbar - its not working for process-spawner/manager/{id}
            processSpawnerMenuItem.route = '/process-spawner/user/' + user.id;
        }

        const userRoles = this.authService.getUserRoles();
        this.menuItems = this.menuItems.filter(item => {
            if (item.role.length === 0) {
                return true;
            }

            return item.role.some(role => userRoles.includes(role));
        });
    }

    private getProcessSpawnerMenuItem(): MenuItem | undefined {
        return this.menuItems.find(item => item.route === '/process-spawner/user/');
    }
}
